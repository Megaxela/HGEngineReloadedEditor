// Editor
#include <Widgets/GameObjects.hpp>
#include <Widgets/CommonSettings.hpp>
#include <Tools/ImGuiIdentificators.hpp>
#include <Tools/ImGuiWidgets.hpp>

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
    ImGui::IDGuard idGuard(HG::ID::GameObject::Window);

    if (ImGui::Begin(HG::Names::GameObject::Window, &m_opened))
    {
        drawGameObjects();
    }

    drawMenu();

    ImGui::End();
}

void HG::Editor::Widgets::GameObjects::drawGameObjects()
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

void HG::Editor::Widgets::GameObjects::drawMenu()
{
    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::IDGuard(HG::ID::GameObject::Menu::Copy),
            ImGui::Selectable(HG::Names::GameObject::Menu::Copy))
        {

        }

        if (ImGui::IDGuard(HG::ID::GameObject::Menu::Paste),
            ImGui::Selectable(HG::Names::GameObject::Menu::Paste))
        {

        }

        ImGui::Separator();

        if (ImGui::IDGuard(HG::ID::GameObject::Menu::CreateEmpty),
            ImGui::Selectable(HG::Names::GameObject::Menu::CreateEmpty))
        {

        }

        {
            ImGui::IDGuard idGuard(HG::ID::GameObject::Menu::Menu3DObject);
            if (ImGui::BeginMenu(HG::Names::GameObject::Menu::Menu3DObject))
            {
                if (ImGui::IDGuard(HG::ID::GameObject::Menu::Cube),
                        ImGui::Selectable(HG::Names::GameObject::Menu::Cube))
                {

                }

                ImGui::EndMenu();
            }
        }

        {
            ImGui::IDGuard idGuard(HG::ID::GameObject::Menu::Menu2DObject);
            if (ImGui::BeginMenu(HG::Names::GameObject::Menu::Menu2DObject))
            {
                if (ImGui::IDGuard(HG::ID::GameObject::Menu::Sprite),
                        ImGui::Selectable(HG::Names::GameObject::Menu::Sprite))
                {

                }

                ImGui::EndMenu();
            }
        }

        {
            ImGui::IDGuard idGuard(HG::ID::GameObject::Menu::MenuLight);
            if (ImGui::BeginMenu(HG::Names::GameObject::Menu::MenuLight))
            {
                if (ImGui::IDGuard(HG::ID::GameObject::Menu::PointLight),
                        ImGui::Selectable(HG::Names::GameObject::Menu::PointLight))
                {

                }

                ImGui::EndMenu();
            }
        }

        {
            ImGui::IDGuard idGuard(HG::ID::GameObject::Menu::MenuAudio);
            if (ImGui::BeginMenu(HG::Names::GameObject::Menu::MenuAudio))
            {

                ImGui::EndMenu();
            }
        }

        {
            ImGui::IDGuard idGuard(HG::ID::GameObject::Menu::MenuUI);
            if (ImGui::BeginMenu(HG::Names::GameObject::Menu::MenuUI))
            {

                ImGui::EndMenu();
            }
        }

        if (ImGui::IDGuard(HG::ID::GameObject::Menu::Camera),
            ImGui::Selectable(HG::Names::GameObject::Menu::Camera))
        {

        }

        ImGui::EndPopup();
    }
}

void HG::Editor::Widgets::GameObjects::displayGameObject(HG::Core::GameObject *gameObject)
{
    auto hasChildren = !gameObject->transform()->children().empty();

    auto nodeFlags =
            (hasChildren ? 0U : ImGuiTreeNodeFlags_Leaf) |
            ImGuiTreeNodeFlags_OpenOnArrow |
            (gameObject == m_commonSettings->selectedGameObject ? ImGuiTreeNodeFlags_Selected : 0U);

    bool opened =
            (ImGui::IDGuard(HG::ID::GameObject::TreeItem),
             ImGui::TreeNodeEx(gameObject, nodeFlags, "%s", gameObject->name().c_str()));

    if (ImGui::BeginDragDropSource())
    {
        m_commonSettings->selectedGameObject = m_previousSelected;

        ImGui::SetDragDropPayload(HG::ID::DragDrop::GameObject, &gameObject, sizeof(void*));

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
