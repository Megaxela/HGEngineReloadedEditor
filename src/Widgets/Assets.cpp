// Editor
#include <Widgets/Assets.hpp>

// ImGui
#include <imgui.h>

HG::Editor::Widgets::Assets::Assets(HG::Editor::Widgets::Settings::Common *settings) :
    m_commonSettings(settings)
{

}

void HG::Editor::Widgets::Assets::onDraw()
{
    if (ImGui::Begin("Assets", &m_opened))
    {

    }
    ImGui::End();
}
