// Editor
#include <Widgets/Inspector.hpp>
#include <Widgets/CommonSettings.hpp>
#include <Editor/Application.hpp>
#include <Fabrics/PropertyEditorsFabric.hpp>
#include <AbstractPropertyProcessor.hpp>
#include <Tools/ImGuiWidgets.hpp>

// HG::Core
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Transform.hpp>

// ImGui
#include <imgui.h>

// ALogger
#include <SystemTools.h>
#include <Tools/ImGuiIdentificators.hpp>

HG::Editor::Widgets::Inspector::Inspector(HG::Editor::Widgets::Settings::Common *settings) :
    m_commonSettings(settings)
{

}

void HG::Editor::Widgets::Inspector::onDraw()
{
    ImGui::IDGuard idGuard(HG::ID::Inspector::Window);

    if (ImGui::Begin(HG::Names::Inspector::Window, &m_opened))
    {
        switch (m_commonSettings->lastSelectedType)
        {
        case HG::Editor::Widgets::Settings::Common::LastSelectedType::GameObject:
            drawGameObjectBody();
            break;
        case HG::Editor::Widgets::Settings::Common::LastSelectedType::Asset:
            drawAssetBody();
            break;
        case HG::Editor::Widgets::Settings::Common::LastSelectedType::None:
            break;
        }
    }

    ImGui::End();
}

void HG::Editor::Widgets::Inspector::drawGameObjectBody()
{
    auto gameObject = m_commonSettings->selectedGameObject;

    if (m_commonSettings->selectedGameObject == nullptr)
    {
        return;
    }

    // Displaying gameobject header
    bool enabledCache = gameObject->isEnabled();
    if (ImGui::IDGuard(HG::ID::Inspector::EnabledCheckbox),
        ImGui::Checkbox(HG::Names::Inspector::EnabledCheckbox, &enabledCache))
    {
        gameObject->setEnabled(enabledCache);
    }

    ImGui::SameLine();

    ImGui::Text("%s", gameObject->name().c_str());

    // todo: Don't invalidate cache if current gameobject was not changed
    //       or if behaviours amount was not changed.
    m_commonSettings->behavioursCache.clear();

    // Getting and iterating all behaviours
    gameObject->getBehaviours(m_commonSettings->behavioursCache);
    gameObject->getRenderingBehaviours(m_commonSettings->behavioursCache);

    // Getting Editor::Application
    auto editorApplication = application();

    if (editorApplication == nullptr)
    {
        ImGui::Text("%s", "This application has wrong type.");
        return;
    }

    drawTransformEdit(gameObject->transform());

    for (const auto& behaviour : m_commonSettings->behavioursCache)
    {
        ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
        if (ImGui::IDGuard(HG::ID::GameObject::Header),
            ImGui::CollapsingHeader(SystemTools::getTypeName(*behaviour).c_str()))
        {
            m_commonSettings->propertiesCache.clear();

            // Getting and iterating all properties
            behaviour->getProperties(m_commonSettings->propertiesCache);

            ImGui::Columns(2);

            for (const auto& property : m_commonSettings->propertiesCache)
            {
                // Displaying title
                ImGui::AlignTextToFramePadding();
                ImGui::Text("%s", property.name().c_str());

                // Prepearing processor
                auto processor = editorApplication
                    ->propertyEditorsFabric()
                    ->create(property.typeInfo().hash_code());

                // Moving to next column
                ImGui::NextColumn();

                if (processor == nullptr)
                {
                    ImGui::Text("No processor for \"%s\" type (%s)", property.type().c_str(), property.name().c_str());
                    continue;
                }

                ImGui::IDGuard idGuard((void*) &property);

                ImGui::PushItemWidth(-1);
                processor->perform(std::string(), property);
                ImGui::PopItemWidth();

                ImGui::NextColumn();
            }

            ImGui::Columns(1);
        }
    }

    ImGui::Separator();

    drawAddBehaviourButton();
}

void HG::Editor::Widgets::Inspector::drawTransformEdit(HG::Core::Transform* transform)
{
    ImGui::Columns(2);

    int id = 0;

    // Position
    ImGui::Text("Position");
    ImGui::NextColumn();

    ImGui::PushItemWidth(-1);
    auto tempVec3 = transform->globalPosition();
    if (ImGui::IDGuard(++id),
        ImGui::DragFloat3("##Position", tempVec3.data.data, 0.05))
    {
        transform->setGlobalPosition(tempVec3);
    }
    ImGui::PopItemWidth();

    ImGui::NextColumn();

    ImGui::Text("Rotation");
    ImGui::NextColumn();

    ImGui::PushItemWidth(-1);
    tempVec3 = glm::eulerAngles(transform->localRotation());
    if (ImGui::IDGuard(++id),
        ImGui::DragFloat3("##Rotation", tempVec3.data.data, 0.05))
    {
        transform->setLocalRotation(glm::quat(tempVec3));
    }
    ImGui::PopItemWidth();

    ImGui::NextColumn();

    ImGui::Text("Scale");
    ImGui::NextColumn();

    ImGui::PushItemWidth(-1);
    tempVec3 = transform->localScale();
    if (ImGui::IDGuard(++id),
        ImGui::DragFloat3("##Scale", tempVec3.data.data, 0.05))
    {
        transform->setLocalScale(tempVec3);
    }
    ImGui::PopItemWidth();

    ImGui::Columns(1);
}

void HG::Editor::Widgets::Inspector::drawAddBehaviourButton()
{
    ImGui::Button("Add Behaviour");
}

void HG::Editor::Widgets::Inspector::drawAssetBody()
{

}
