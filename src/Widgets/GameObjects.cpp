// Editor
#include <Widgets/GameObjects.hpp>
#include <Widgets/CommonSettings.hpp>

// HG::Core
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Transform.hpp>

// ImGui
#include <imgui.h>

HG::Editor::Widgets::GameObjects::GameObjects(HG::Editor::Widgets::Settings::Common *settings) :
    m_commonSettings(settings)
{

}

void HG::Editor::Widgets::GameObjects::onDraw()
{
    if (ImGui::Begin("GameObjects", &m_opened))
    {
        for (auto&& gameObject : m_commonSettings->gameobjectsCache)
        {
            if (gameObject->transform()->parent() != nullptr)
            {
                continue;
            }

            displayGameObject(gameObject);
        }
    }
    ImGui::End();
}

void HG::Editor::Widgets::GameObjects::displayGameObject(HG::Core::GameObject *gameObject)
{
    auto hasChildren = !gameObject->transform()->children().empty();

    auto nodeFlags =
            (hasChildren ? 0U : ImGuiTreeNodeFlags_Leaf) |
            ImGuiTreeNodeFlags_OpenOnArrow |
            (gameObject == m_commonSettings->selectedGameObject ? ImGuiTreeNodeFlags_Selected : 0U);

    bool opened = ImGui::TreeNodeEx(gameObject, nodeFlags, "%s", gameObject->name().c_str());

    // If LMB pressed on this item
    if (ImGui::IsItemClicked(0))
    {
        m_commonSettings->lastSelectedType = HG::Editor::Widgets::Settings::Common::LastSelectedType::GameObject;
        m_commonSettings->selectedGameObject = gameObject;
    }

    // If this gameobject opened - show
    // it's children.
    if (opened)
    {
        for (auto&& childTransform : gameObject->transform()->children())
        {
            displayGameObject(childTransform->gameObject());
        }

        ImGui::TreePop();
    }
}
