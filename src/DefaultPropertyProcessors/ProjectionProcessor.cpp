// Editor
#include <DefaultPropertyProcessors/ProjectionProcessor.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Camera.hpp>

// ImGui
#include <imgui.h>

void HG::Editor::PropertyProcessors::ProjectionProcessor::perform(std::size_t id,
                                                                  const HG::Core::Behaviour::Property &property)
{
    auto key = "##" + property.name() + std::to_string(id);

    auto value = static_cast<int>(property.getGetter<HG::Rendering::Base::Camera::Projection>()());

    const char* items[] = {"Perspective", "Orthogonal"};

    if (ImGui::Combo(key.c_str(), &value, items, 2))
    {
        property.getSetter<HG::Rendering::Base::Camera::Projection>()(
            static_cast<HG::Rendering::Base::Camera::Projection>(value)
        );
    }
}
