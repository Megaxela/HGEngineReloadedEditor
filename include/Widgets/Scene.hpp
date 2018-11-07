#pragma once

// Editor
#include <Widgets/AbstractWidget.hpp>

// glm
#include <glm/vec2.hpp>

namespace HG::Rendering::Base
{
    class RenderTarget;
}

namespace HG::Editor::Widgets
{
    /**
     * @brief Class, that describes Scene widget.
     */
    class Scene : public AbstractWidget
    {
    public:

        /**
         * @brief Constructor.
         */
        Scene();

        /**
         * @brief Method for setting render target to display.
         * @param target
         */
        void setRenderTarget(HG::Rendering::Base::RenderTarget* target);

    protected:
        void onDraw() override;

    private:

        void updateRenderOverride();

        glm::ivec2 m_size;

        HG::Rendering::Base::RenderTarget* m_mainRenderTarget;
    };
}
