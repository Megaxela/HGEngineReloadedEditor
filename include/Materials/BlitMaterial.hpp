#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/Material.hpp>

// HG::Utils
#include <HG/Utils/Color.hpp>

namespace HG::Editor::Materials
{
    /**
     * @brief Class, that describes material for
     * blitting one texture to another.
     */
    class BlitMaterial : public HG::Rendering::Base::Material
    {
    public:

        static const char* rawShader;

        /**
         * @brief Texture to render.
         * @param texture Pointer to texture.
         */
        void setTexture(HG::Rendering::Base::Texture* texture);
    };
}


