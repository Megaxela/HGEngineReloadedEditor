// Editor
#include <Widgets/InformationModal.hpp>
#include <Tools/ImGuiIdentificators.hpp>
#include <Tools/ImGuiWidgets.hpp>
#include <Editor/Application.hpp>
#include <Tools/GlobalThumbnails.hpp>

// HG::Core
#include <HG/Core/ResourceManager.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Texture.hpp>

// HG::Utils
#include <HG/Utils/Loaders/STBImageLoader.hpp>



HG::Editor::Widgets::InformationModal::InformationModal() :
    m_currentOpenState(false),
    m_type(Type::Info),
    m_text()
{
    setOpened(false);
}

HG::Editor::Widgets::InformationModal::~InformationModal()
{

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

    ImGui::SetNextWindowSize({300, 200}, ImGuiCond_FirstUseEver);
    if (ImGui::BeginPopupModal(HG::Names::InformationModal::Window,
                               nullptr))
    {
        if (!isOpened())
        {
            ImGui::CloseCurrentPopup();
        }

        auto icon = HG::Editor::ThumbnailsCache::InvalidHandle;

        auto* globalThumbnails = application()->globalThumbnails();

        switch (m_type)
        {
        case Type::Info:
            icon = globalThumbnails
                ->getHandle(HG::Editor::GlobalThumbnails::Thumbs::InfoMark);
            break;
        case Type::Warning:
            icon = globalThumbnails
                ->getHandle(HG::Editor::GlobalThumbnails::Thumbs::WarningMark);
            break;
        case Type::Error:
            icon = globalThumbnails
                ->getHandle(HG::Editor::GlobalThumbnails::Thumbs::ErrorMark);
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

        ImGui::SameLine();

        // Text region
        ImGui::BeginGroup();
        {
            ImGui::BeginChild("TextChild", {0, -ImGui::GetFrameHeightWithSpacing()});

            ImGui::TextWrapped("%s", m_text.c_str());

            ImGui::EndChild();

            if (ImGui::Button("Ok"))
            {
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndGroup();

        ImGui::EndPopup();
    }
}
