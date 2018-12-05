// Editor
#include <Widgets/TextureCacheView.hpp>
#include <Tools/ImGuiWidgets.hpp>
#include <Tools/ImGuiIdentificators.hpp>
#include <Editor/Application.hpp>

// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/ResourceCache.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Texture.hpp>

// ImGui
#include <imgui.h>

HG::Editor::Widgets::TextureCacheView::TextureCacheView()
{

}

void HG::Editor::Widgets::TextureCacheView::onDraw()
{
    auto flags = ImGuiWindowFlags_MenuBar;

    ImGui::IDGuard guard(HG::ID::TextureCacheView::Window);

    if (ImGui::Begin(HG::Names::TextureCacheView::Window, &m_opened, flags))
    {
        drawToolbar();

        drawTextures();
    }

    ImGui::End();
}

void HG::Editor::Widgets::TextureCacheView::drawToolbar()
{
    ImGui::IDGuard guard(HG::ID::TextureCacheView::Toolbar);

    ImGui::BeginMenuBar();


    ImGui::EndMenuBar();
}

void HG::Editor::Widgets::TextureCacheView::drawTextures()
{
    // Receiving current resources
    auto textures = application()
            ->resourceCache()
            ->getUsedResources<HG::Rendering::Base::Texture>();

    // todo: Add scrollable
    const std::size_t buttonSideSize = 150;
    // Getting window size
    const auto availableSize = ImGui::GetContentRegionAvailWidth();
    const auto itemsInRow = (int) availableSize / (int) buttonSideSize;

    int rowIndex = 0;

    // Displaying thumbnails
    for (auto& texture : textures)
    {
        if (ImGui::ImageButton(texture, {buttonSideSize, buttonSideSize}))
        {
            m_selectedTexture = texture;
        }

        rowIndex += 1;

        if (rowIndex != itemsInRow)
        {
            ImGui::SameLine();
        }
        else
        {
            rowIndex = 0;
        }
    }
}
