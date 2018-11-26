// Editor
#include <Widgets/InformationModal.hpp>
#include <Tools/ImGuiIdentificators.hpp>
#include <Tools/ImGuiWidgets.hpp>
#include <Editor/Application.hpp>

// HG::Core
#include <HG/Core/ResourceManager.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Texture.hpp>

// HG::Utils
#include <HG/Utils/Loaders/STBImageLoader.hpp>



HG::Editor::Widgets::InformationModal::InformationModal() :
    m_infoIconHandle(HG::Editor::ThumbnailsCache::InvalidHandle),
    m_warningIconHandle(HG::Editor::ThumbnailsCache::InvalidHandle),
    m_errorIconHandle(HG::Editor::ThumbnailsCache::InvalidHandle),
    m_resourcesToFree(),
    m_currentOpenState(false),
    m_type(Type::Info),
    m_text()
{
    setOpened(false);
}

HG::Editor::Widgets::InformationModal::~InformationModal()
{
    application()->thumbnailsCache()->removeThumbnail(m_infoIconHandle);
    application()->thumbnailsCache()->removeThumbnail(m_warningIconHandle);
    application()->thumbnailsCache()->removeThumbnail(m_errorIconHandle);
}

void HG::Editor::Widgets::InformationModal::info(std::string text)
{
    setOpened(true);
    m_text = std::move(text);
    m_type = Type::Info;
}

void HG::Editor::Widgets::InformationModal::warning(std::string text)
{
    setOpened(true);
    m_text = std::move(text);
    m_type = Type::Warning;
}

void HG::Editor::Widgets::InformationModal::error(std::string text)
{
    setOpened(true);
    m_text = std::move(text);
    m_type = Type::Error;
}

void HG::Editor::Widgets::InformationModal::onDraw()
{
    ImGui::IDGuard idGuard(HG::ID::InformationModal::Window);

    if (m_currentOpenState != isOpened())
    {
        m_currentOpenState = isOpened();

        if (isOpened())
        {
            ImGui::OpenPopup(HG::Names::InformationModal::Window);
        }
    }

    if (ImGui::BeginPopupModal(HG::Names::InformationModal::Window,
                               nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (!isOpened())
        {
            ImGui::CloseCurrentPopup();
        }

        auto icon = HG::Editor::ThumbnailsCache::InvalidHandle;

        switch (m_type)
        {
        case Type::Info:
            icon = m_infoIconHandle;
            break;
        case Type::Warning:
            icon = m_warningIconHandle;
            break;
        case Type::Error:
            icon = m_errorIconHandle;
            break;
        }

        HG::Rendering::Base::Texture* thumbnailsTexture = nullptr;
        ImVec2 uvTL;
        ImVec2 uvBR;
        ImVec2 imageSize = {0, 0};

        if (application()->thumbnailsCache()->isAvailable(icon))
        {
            thumbnailsTexture = application()->thumbnailsCache()->texture();
            auto tlbr = application()->thumbnailsCache()->thumbnailTLBR(icon);

            uvTL = ImGui::fromGLM(application()->thumbnailsCache()->pixelsToUV(tlbr.tl));
            uvBR = ImGui::fromGLM(application()->thumbnailsCache()->pixelsToUV(tlbr.br));

            imageSize = ImGui::fromGLM(tlbr.br - tlbr.tl);
        }

        // Child region for icon
        {
            ImGui::BeginChild("IconChild", {imageSize.x + 10.0f, 0});

            ImGui::Image(thumbnailsTexture, imageSize, uvTL, uvBR);

            ImGui::EndChild();
        }
//
//        // Text region
//        {
//            ImGui::BeginChild("TextChild");
//
//            ImGui::Text("%s", m_text.c_str());
//
//            ImGui::EndChild();
//        }
//        ImGui::TextWrapped("%s", m_text.c_str());

        ImGui::EndPopup();
    }
}

void HG::Editor::Widgets::InformationModal::onInitialization()
{
    auto errorTexture = new HG::Rendering::Base::Texture(
        application()
            ->resourceManager()
            ->load<HG::Utils::STBImageLoader>("images/error.png")
            .guaranteeGet()
    );

    m_resourcesToFree.push_back(errorTexture);

    m_errorIconHandle = application()->thumbnailsCache()->addThumbnail(errorTexture);
    m_infoIconHandle = m_errorIconHandle;
    m_warningIconHandle = m_errorIconHandle;

    // Cache invalidation will be performed in GraphicsInterface behaviour
    // after initialization.
}

void HG::Editor::Widgets::InformationModal::onPostInitialization()
{
    // Resources, allocated at initalization side was used
    // by thumbnails cache. Now they can be freed.
    for (auto& resource : m_resourcesToFree)
    {
        delete resource;
    }
}
