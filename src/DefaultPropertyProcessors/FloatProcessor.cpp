// Editor
#include <DefaultPropertyProcessors/FloatProcessor.hpp>

// ImGui
#include <imgui.h>

void HG::Editor::PropertyProcessors::FloatProcessor::perform(const std::string &name, const HG::Core::Behaviour::Property &property)
{
    auto value = property.getGetter<float>()();

    if (ImGui::DragFloat(name.c_str(), &value))
    {
        property.getSetter<float>()(value);
    }
}
