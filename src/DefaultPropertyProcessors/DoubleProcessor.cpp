// Editor
#include <DefaultPropertyProcessors/DoubleProcessor.hpp>

// ImGui
#include <imgui.h>

void HG::Editor::PropertyProcessors::DoubleProcessor::perform(std::size_t id, const std::string &name, const HG::Core::Behaviour::Property &property)
{
    // Creating unique key
    auto key = name + "##" + std::to_string(id);

    auto value = property.getGetter<double>()();

    if (ImGui::DragScalar(key.c_str(), ImGuiDataType_Double, &value, 0.1f))
    {
        property.getSetter<double>()(value);
    }
}
