// Editor
#include <DefaultPropertyProcessors/DoubleProcessor.hpp>

// ImGui
#include <imgui.h>

void HG::Editor::PropertyProcessors::DoubleProcessor::perform(const std::string &name, const HG::Core::Behaviour::Property &property)
{
    auto value = property.getGetter<double>()();

    if (ImGui::DragScalar(name.c_str(), ImGuiDataType_Double, &value, 0.1f))
    {
        property.getSetter<double>()(value);
    }
}
