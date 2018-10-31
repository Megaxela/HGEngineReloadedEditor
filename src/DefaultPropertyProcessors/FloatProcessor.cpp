// Editor
#include <DefaultPropertyProcessors/FloatProcessor.hpp>

// ImGui
#include <imgui.h>

void HG::Editor::PropertyProcessors::FloatProcessor::perform(std::size_t id, const HG::Core::Behaviour::Property &property)
{
    // Creating unique key
    auto key = "##" + property.name() + std::to_string(id);

    auto value = property.getGetter<float>()();

    if (ImGui::DragFloat(key.c_str(), &value))
    {
        property.getSetter<float>()(value);
    }
}
