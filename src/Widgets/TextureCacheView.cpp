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

    for (auto& texture : textures)
    {
        ImGui::Image(texture, {600, 600});
    }
}
