// Editor
#include <DefaultPropertyProcessors/ProjectionProcessor.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Camera.hpp>

// ImGui
#include <imgui.h>

void HG::Editor::PropertyProcessors::ProjectionProcessor::perform(const std::string &name, const HG::Core::Behaviour::Property &property)
{
    auto value = static_cast<int>(property.getGetter<HG::Rendering::Base::Camera::Projection>()());

    const char* items[] = {"Perspective", "Orthogonal"};

    if (ImGui::Combo(name.c_str(), &value, items, 2))
    {
        property.getSetter<HG::Rendering::Base::Camera::Projection>()(
            static_cast<HG::Rendering::Base::Camera::Projection>(value)
        );
    }
}
