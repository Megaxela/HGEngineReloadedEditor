// Editor
#include <DefaultPropertyProcessors/ColorProcessor.hpp>

// HG::Utils
#include <HG/Utils/Color.hpp>

// ImGui
#include <imgui.h>

void HG::Editor::PropertyProcessors::ColorProcessor::perform(std::size_t id, const std::string &name, const HG::Core::Behaviour::Property &property)
{
    // Creating unique key
    auto key = name + "##" + std::to_string(id);

    auto value = property.getGetter<HG::Utils::Color>()();

    if (ImGui::ColorEdit4(key.c_str(), (float*) &value, ImGuiColorEditFlags_RGB))
    {
        property.getSetter<HG::Utils::Color>()(value);
    }
}
