#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/Material.hpp>

// HG::Utils
#include <HG/Utils/Color.hpp>

namespace HG::Editor::Materials
{
    /**
     * @brief Class, that describes material for overriding
     * rendering for selection.
     */
    class ColorMaterial : public HG::Rendering::Base::Material
    {
    public:

        static const char* rawShader;

        void setColor(const HG::Utils::Color& color);
    };
}

