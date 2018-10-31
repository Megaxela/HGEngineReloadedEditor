// Editor
#include <DefaultPropertyProcessors/DoubleProcessor.hpp>

// ImGui
#include <imgui.h>

void HG::Editor::PropertyProcessors::DoubleProcessor::perform(std::size_t id, const HG::Core::Behaviour::Property &property)
{
    // Creating unique key
    auto key = "##" + property.name() + std::to_string(id);

    auto value = static_cast<float>(property.getGetter<double>()());

    if (ImGui::DragFloat(key.c_str(), &value))
    {
        property.getSetter<double>()(value);
    }
}
