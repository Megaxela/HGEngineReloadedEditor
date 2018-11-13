#pragma once

// Editor
#include <Widgets/AbstractWidget.hpp>

// HG::Utils
#include <HG/Utils/Color.hpp>

// glm
#include <glm/vec2.hpp>

namespace HG::Rendering::Base
{
    class RenderTarget;
    class RenderOverride;
}

namespace HG::Core
{
    class GameObject;
}

namespace HG::Editor::Widgets::Settings
{
    struct Common;
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
        explicit Scene(HG::Editor::Widgets::Settings::Common* common);

        /**
         * @brief Destructor.
         */
        ~Scene() override;

        /**
         * @brief Method for setting render target to display.
         * @param target
         */
        void setRenderTarget(HG::Rendering::Base::RenderTarget* target);

    protected:
        void onDraw() override;

    private:

        void updateRenderOverride();

        HG::Core::GameObject* checkSelectedGameObject();

        glm::ivec2 m_size;

        HG::Rendering::Base::RenderTarget* m_mainRenderTarget;

        HG::Rendering::Base::RenderOverride* m_selectionOverride;

        HG::Editor::Widgets::Settings::Common* m_commonSettings;

        std::unordered_map<HG::Utils::Color, HG::Core::GameObject*> m_colorCache;
    };
}
