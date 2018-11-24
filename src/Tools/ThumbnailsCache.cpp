// Editor
#include <Tools/ThumbnailsCache.hpp>
#include <Editor/Application.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Texture.hpp>
#include <HG/Rendering/Base/RenderTarget.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/RenderingPipeline.hpp>

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

HG::Editor::Application *HG::Editor::ThumbnailsCache::parentApplication() const
{
    return m_parentApplication;
}

HG::Rendering::Base::Texture *HG::Editor::ThumbnailsCache::texture() const
{
    return m_texture;
}

HG::Editor::ThumbnailsCache::Handler HG::Editor::ThumbnailsCache::addThumbnail(HG::Rendering::Base::Texture *texture, glm::ivec2 size)
{
    m_newOrUpdateThumbnails[++m_handlerCounter] = NewThumbnail(
        texture,
        (size.x == 0 || size.y == 0) ? texture->size() : size
    );

    return m_handlerCounter;
}

void HG::Editor::ThumbnailsCache::updateThumbnail(HG::Editor::ThumbnailsCache::Handler handler,
                                                  HG::Rendering::Base::Texture *texture, glm::ivec2 size)
{
    if (m_newOrUpdateThumbnails.count(handler) == 0 &&
        m_currentThumbnails.count(handler))
    {
        throw std::invalid_argument("No thumbnail with provided handler");
    }

    m_newOrUpdateThumbnails[handler] = NewThumbnail(
        texture,
        (size.x == 0 || size.y == 0) ? texture->size() : size
    );
}

void HG::Editor::ThumbnailsCache::removeThumbnail(HG::Editor::ThumbnailsCache::Handler handler)
{
    m_newOrUpdateThumbnails.erase(handler);
    m_currentThumbnails.erase(handler);
}

bool HG::Editor::ThumbnailsCache::isAvailable(HG::Editor::ThumbnailsCache::Handler handler) const
{
    return m_currentThumbnails.count(handler) > 0;
}

HG::Editor::ThumbnailsCache::TLBR HG::Editor::ThumbnailsCache::thumbnailTLBR(HG::Editor::ThumbnailsCache::Handler handler) const
{
    auto iterator = m_currentThumbnails.find(handler);

    if (iterator == m_currentThumbnails.end())
    {
        throw std::runtime_error("Thumbnail with provided handler is not available");
    }

    return iterator->second;
}

void HG::Editor::ThumbnailsCache::invalidateCache()
{
    Info() << "Invalidating cache";

    constexpr bool allow_flip = false;
    const auto runtime_flipping_mode = rectpack2D::flipping_option::DISABLED;

    using spaces_type = rectpack2D::empty_spaces<allow_flip, rectpack2D::default_empty_spaces>;
    using rect_type = rectpack2D::output_rect_t<spaces_type>;

    // Side will be multiplied by 2 on initial stage.
    auto max_side = (m_texture != nullptr) ? (m_texture->size().x / 2) : (1024 / 2);

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
    std::vector<Handler> handlers;

    // Inserting current thumbnails
    for (const auto& [handler, tlbr] : m_currentThumbnails)
    {
        // If it's in new - it's update. Skip.
        if (m_newOrUpdateThumbnails.count(handler))
        {
            continue;
        }

        auto size = tlbr.tl - tlbr.br;

        rectangles.emplace_back(rect_type(0, 0, size.x, size.y));
        handlers.emplace_back(handler);
    }

    // Inserting new/update thumbnails
    for (const auto& [handler, thumbnail] : m_newOrUpdateThumbnails)
    {
        rectangles.emplace_back(rect_type(0, 0, thumbnail.size.x, thumbnail.size.y));
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

    std::unordered_map<Handler, TLBR> newCurrent;

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

    auto newTexture = new HG::Rendering::Base::Texture(
        {max_side, max_side},
        HG::Rendering::Base::Texture::Format::RGBA
    );

    auto renderTarget = new HG::Rendering::Base::RenderTarget({max_side, max_side});
    renderTarget->setColorTexture(newTexture);

    loadCurrentThumbnails(renderTarget, newCurrent);


}

void HG::Editor::ThumbnailsCache::loadCurrentThumbnails(HG::Rendering::Base::RenderTarget *texture,
                                                        const std::unordered_map<HG::Editor::ThumbnailsCache::Handler, HG::Editor::ThumbnailsCache::TLBR> &newCurrent)
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

//        parentApplication()
//            ->renderer()
//            ->pipeline()
//            ->render()
    }
}
