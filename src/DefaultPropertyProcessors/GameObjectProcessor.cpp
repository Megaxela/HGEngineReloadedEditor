// Editor
#include <DefaultPropertyProcessors/GameObjectProcessor.hpp>
#include <Tools/ImGuiIdentificators.hpp>

// HG::Core
#include <HG/Core/GameObject.hpp>

// ImGui
#include <imgui.h>

void HG::Editor::PropertyProcessors::GameObjectProcessor::perform(std::size_t id, const std::string &name, const HG::Core::Behaviour::Property &property)
{
    auto key = name + "##" + std::to_string(id);

    auto value = property.getGetter<HG::Core::GameObject*>()();

    std::string behaviourName = "None";

    if (value != nullptr)
    {
        behaviourName = value->name();
    }

    if (ImGui::Button(behaviourName.c_str()))
    {

    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(HG::Identificators::DragDrop::GameObject))
        {
            auto object = *static_cast<HG::Core::GameObject**>(payload->Data);

            property.getSetter<HG::Core::GameObject*>()(object);
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::SameLine();

    ImGui::Text("%s", name.c_str());
}
