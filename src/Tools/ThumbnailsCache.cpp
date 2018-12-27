// Editor
#include <Tools/ThumbnailsCache.hpp>
#include <Editor/Application.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Texture.hpp>
#include <HG/Rendering/Base/RenderTarget.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>
#include <HG/Rendering/Base/BlitData.hpp>

// rectpack2D
#include <finders_interface.h>

// ALogger
#include <CurrentLogger.hpp>



HG::Editor::ThumbnailsCache::ThumbnailsCache(HG::Editor::Application *application) :
    m_parentApplication(application),
    m_texture(nullptr),
    m_currentThumbnails(),
    m_newOrUpdateThumbnails(),
    m_handlerCounter(0)
{

}

HG::Editor::Application *HG::Editor::ThumbnailsCache::application() const
{
    return m_parentApplication;
}

HG::Rendering::Base::Texture *HG::Editor::ThumbnailsCache::texture() const
{
    std::shared_lock<std::shared_mutex> lock(m_textureMutex);
    return m_texture;
}

HG::Editor::ThumbnailsCache::Handle HG::Editor::ThumbnailsCache::addThumbnail(HG::Rendering::Base::Texture *texture, glm::ivec2 size)
{
    std::unique_lock<std::shared_mutex> lock(m_newOrUpdateMutex);
    m_newOrUpdateThumbnails[++m_handlerCounter] = NewThumbnail(texture, size);

    return m_handlerCounter;
}

void HG::Editor::ThumbnailsCache::updateThumbnail(HG::Editor::ThumbnailsCache::Handle handler,
                                                  HG::Rendering::Base::Texture *texture, glm::ivec2 size)
{

    std::unique_lock<std::shared_mutex> lock(m_newOrUpdateMutex);

    {
        std::shared_lock<std::shared_mutex> currentLock(m_currentMutex);
        if (m_newOrUpdateThumbnails.count(handler) == 0 &&
            m_currentThumbnails.count(handler))
        {
            throw std::invalid_argument("No thumbnail with provided handler");
        }
    }

    m_newOrUpdateThumbnails[handler] = NewThumbnail(texture, size);
}

void HG::Editor::ThumbnailsCache::removeThumbnail(HG::Editor::ThumbnailsCache::Handle handler)
{
    std::unique_lock<std::shared_mutex> newLock(m_newOrUpdateMutex);
    std::unique_lock<std::shared_mutex> currentLock(m_currentMutex);

    m_newOrUpdateThumbnails.erase(handler);
    m_currentThumbnails.erase(handler);
}

bool HG::Editor::ThumbnailsCache::isAvailable(HG::Editor::ThumbnailsCache::Handle handler) const
{
    std::shared_lock<std::shared_mutex> currentLock(m_currentMutex);
    return m_currentThumbnails.count(handler) > 0;
}

HG::Editor::ThumbnailsCache::TLBR HG::Editor::ThumbnailsCache::thumbnailTLBR(HG::Editor::ThumbnailsCache::Handle handler) const
{
    std::shared_lock<std::shared_mutex> currentLock(m_currentMutex);
    auto iterator = m_currentThumbnails.find(handler);

    if (iterator == m_currentThumbnails.end())
    {
        throw std::runtime_error("Thumbnail with provided handler is not available");
    }

    return iterator->second;
}

glm::vec2 HG::Editor::ThumbnailsCache::pixelsToUV(glm::ivec2 value)
{
    std::shared_lock<std::shared_mutex> lock(m_textureMutex);
    if (m_texture == nullptr)
    {
        return glm::vec2(0, 0);
    }

    return glm::vec2(value) / glm::vec2(m_texture->size());
}

bool HG::Editor::ThumbnailsCache::invalidationRequired() const
{
    std::shared_lock<std::shared_mutex> newOrUpdate(m_newOrUpdateMutex);

    return !m_newOrUpdateThumbnails.empty();
}

void HG::Editor::ThumbnailsCache::invalidateCache()
{
    Info() << "Invalidating cache";

    auto beginTime = std::chrono::steady_clock::now();

    std::unique_lock<std::shared_mutex> textureLock(m_textureMutex);
    std::unique_lock<std::shared_mutex> currentLock(m_currentMutex);
    std::unique_lock<std::shared_mutex> newOrUpdate(m_newOrUpdateMutex);

    if (m_newOrUpdateThumbnails.empty())
    {
        Info() << "Cache invalidating called, but no changes apply.";
        return;
    }

    constexpr bool allow_flip = false;
    const auto runtime_flipping_mode = rectpack2D::flipping_option::DISABLED;

    using spaces_type = rectpack2D::empty_spaces<allow_flip, rectpack2D::default_empty_spaces>;
    using rect_type = rectpack2D::output_rect_t<spaces_type>;

    // Side will be multiplied by 2 on initial stage.
    auto max_side = (m_texture != nullptr) ? (m_texture->size(true).x / 2) : (1024 / 2);

    bool success = true;

    auto report_successful = [](rect_type&)
    { return rectpack2D::callback_result::CONTINUE_PACKING; };

    auto report_unsuccessful = [&success](rect_type&)
    {
        success = false;
        return rectpack2D::callback_result::ABORT_PACKING;
    };

    const auto discard_step = 1;

    std::vector<rect_type> rectangles;
    std::vector<Handle> handlers;

    // Inserting current thumbnails
    for (const auto& [handler, tlbr] : m_currentThumbnails)
    {
        // If it's in new - it's update. Skip.
        if (m_newOrUpdateThumbnails.count(handler))
        {
            continue;
        }

        auto size = tlbr.br - tlbr.tl;

        rectangles.emplace_back(rect_type(0, 0, size.x, size.y));
        handlers.emplace_back(handler);
    }

    // Inserting new/update thumbnails
    for (const auto& [handler, thumbnail] : m_newOrUpdateThumbnails)
    {
        auto size = thumbnail.size;

        if (size == glm::ivec2(0, 0))
        {
            size = thumbnail.texture->size(true);
        }

        rectangles.emplace_back(rect_type(0, 0, size.x, size.y));
        handlers.emplace_back(handler);
    }

    Info() << "Packing " << rectangles.size() << " thumbnails";

    success = false;

    while (!success)
    {
        success = true;
        max_side *= 2;

        Info() << "Trying " << max_side << "x" << max_side << " texture side.";

        // Executing
        const auto result_size = rectpack2D::find_best_packing<spaces_type>(
            rectangles,
            rectpack2D::make_finder_input(
                max_side,
                discard_step,
                report_successful,
                report_unsuccessful,
                runtime_flipping_mode
            )
        );
    }

    std::unordered_map<Handle, TLBR> newCurrent;

    // Creating map from vectors
    for (std::size_t index = 0; index < rectangles.size(); ++index)
    {
        const auto rect = rectangles[index];

        newCurrent[handlers[index]] = {
            {rect.x,          rect.y         },
            {rect.x + rect.w, rect.y + rect.h}
        };
    }

    Info() << "Packing finished. Loading to GPU.";

    auto newTexture = new (application()->resourceCache()) HG::Rendering::Base::Texture(
        {max_side, max_side},
        HG::Rendering::Base::Texture::Format::RGBA
    );

    auto renderTarget = new HG::Rendering::Base::RenderTarget({max_side, max_side});
    renderTarget->setColorTexture(newTexture);


    HG::Rendering::Base::BlitData blitData;

    prepareCurrentThumbnails(blitData, newCurrent);
    prepareNewThumbnails(blitData, newCurrent);

    // Saving rendertarget state
    auto currentRenderTarget = application()->renderer()->pipeline()->renderTarget();
    auto override = application()->renderer()->pipeline()->renderOverride();

    // Setting new render target
    application()->renderer()->pipeline()->setRenderOverride(nullptr);
    application()->renderer()->pipeline()->setRenderTarget(renderTarget);

    // Clearing new render target
    application()->renderer()->pipeline()->clear(HG::Utils::Color::Transparent);

    // Perform blitting
    application()->renderer()->pipeline()->blit(renderTarget, &blitData);

    // Restoring rendertarget state
    application()->renderer()->pipeline()->setRenderTarget(currentRenderTarget);
    application()->renderer()->pipeline()->setRenderOverride(override);

    // Applying new data
    m_currentThumbnails = newCurrent;
    m_newOrUpdateThumbnails.clear();

    delete m_texture;
    m_texture = newTexture;

    auto finishTime = std::chrono::steady_clock::now();

    Info() << "Loading finished. It took " << std::chrono::duration_cast<std::chrono::milliseconds>(finishTime - beginTime).count() << "ms";
}

void HG::Editor::ThumbnailsCache::prepareCurrentThumbnails(HG::Rendering::Base::BlitData &blitData,
                                                           const HG::Editor::ThumbnailsCache::HandlerTLBRContainer &newCurrent)
{
    if (m_texture == nullptr)
    {
        return;
    }

    for (const auto& [handler, tlbr] : m_currentThumbnails)
    {
        const auto& iter = newCurrent.find(handler);

        if (iter == newCurrent.end())
        {
            continue;
        }

        blitData.blitRectangular(
            m_texture,
            tlbr.tl,
            tlbr.br,
            iter->second.tl
        );
    }
}

void HG::Editor::ThumbnailsCache::prepareNewThumbnails(HG::Rendering::Base::BlitData &blitData,
                                                       const HG::Editor::ThumbnailsCache::HandlerTLBRContainer &newCurrent)
{
    for (const auto& [handler, newThumbnail] : m_newOrUpdateThumbnails)
    {
        const auto& iter = newCurrent.find(handler);

        if (iter == newCurrent.end())
        {
            continue;
        }

        blitData.blitRectangular(
            newThumbnail.texture,
            {0, 0},
            newThumbnail.texture->size(),
            iter->second.tl,
            iter->second.br - iter->second.tl
        );
    }
}
