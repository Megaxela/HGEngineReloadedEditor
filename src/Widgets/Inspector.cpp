// Editor
#include <Widgets/Inspector.hpp>
#include <Widgets/CommonSettings.hpp>
#include <Editor/Application.hpp>
#include <Fabrics/PropertyEditorsFabric.hpp>
#include <AbstractPropertyProcessor.hpp>

// HG::Core
#include <HG/Core/GameObject.hpp>

// ImGui
#include <imgui.h>

HG::Editor::Widgets::Inspector::Inspector(HG::Editor::Widgets::Settings::Common *settings) :
    m_commonSettings(settings)
{

}

void HG::Editor::Widgets::Inspector::onDraw()
{
    if (ImGui::Begin("Inspector", &m_opened))
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
    if (ImGui::Checkbox("##InspectorEnabled", &enabledCache))
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

    std::size_t id = 0;

    for (const auto& behaviour : m_commonSettings->behavioursCache)
    {
        m_commonSettings->propertiesCache.clear();

        // Getting and iterating all properties
        behaviour->getProperties(m_commonSettings->propertiesCache);

        for (const auto& property : m_commonSettings->propertiesCache)
        {
            // Displaying title
            auto processor = editorApplication
                    ->propertyEditorsFabric()
                    ->create(property.typeInfo().hash_code());

            if (processor == nullptr)
            {
                ImGui::Text("No processor for \"%s\" type (%s)", property.type().c_str(), property.name().c_str());
                continue;
            }

            processor->perform(id++, property.name(), property);
        }
    }
}

void HG::Editor::Widgets::Inspector::drawAssetBody()
{

}
