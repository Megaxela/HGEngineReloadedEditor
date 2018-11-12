// Editor
#include <Widgets/GameObjects.hpp>
#include <Widgets/CommonSettings.hpp>
#include <Tools/ImGuiIdentificators.hpp>

// HG::Core
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Transform.hpp>

// ImGui
#include <imgui.h>
#include <CurrentLogger.hpp>

HG::Editor::Widgets::GameObjects::GameObjects(HG::Editor::Widgets::Settings::Common *settings) :
    m_commonSettings(settings),
    m_previousSelected(nullptr)
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

    if (ImGui::BeginDragDropSource())
    {
        m_commonSettings->selectedGameObject = m_previousSelected;

        ImGui::SetDragDropPayload(HG::Identificators::DragDrop::GameObject, &gameObject, sizeof(void*));

        ImGui::Text("GameObject: %s", gameObject->name().c_str());

        ImGui::EndDragDropSource();
    }

    // If LMB pressed on this item
    if (ImGui::IsItemClicked(0))
    {
        m_previousSelected = (m_previousSelected == nullptr ? gameObject : m_commonSettings->selectedGameObject);
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
