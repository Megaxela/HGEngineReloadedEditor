#pragma once

// C++ STL
#include <unordered_map>

// Editor
#include <Widgets/AbstractWidget.hpp>

// HG::Utils
#include <HG/Utils/Color.hpp>

// glm
#include <glm/vec2.hpp>
#include <imgui.h>

namespace HG::Editor::Materials
{
    class ColorMaterial;
}

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

        /**
         * @brief Method for casting global position to scene position.
         * @param vec Global position.
         * @return Scene position.
         */
        glm::ivec2 globalPosToScenePos(const glm::ivec2& pos) const;

        /**
         * @brief Method for checking is global position in
         * scene's widget.
         * @param pos Global position.
         */
        bool isGlobalPosInScene(const glm::ivec2& pos) const;

    protected:

        /**
         * @brief Method, that performs drawing of render target.
         */
        void onDraw() override;

    private:

        /**
         * @brief Method for updating render override system size
         * according to widget size.
         */
        void updateRenderOverride();

        /**
         * @brief Method for getting gameobject by scene position.
         * @param vec2 Scene position.
         * @return Pointer to GameObject or nullptr if there was no
         * object.
         */
        HG::Core::GameObject* checkSelectedGameObject(const glm::ivec2& vec2);

        glm::ivec2 m_position;
        glm::ivec2 m_size;

        HG::Rendering::Base::RenderTarget* m_mainRenderTarget;

        HG::Rendering::Base::RenderOverride* m_selectionOverride;
        HG::Editor::Materials::ColorMaterial* m_materialOverride;

        HG::Editor::Widgets::Settings::Common* m_commonSettings;

        std::unordered_map<std::size_t, HG::Core::GameObject*> m_colorCache;
    };
}
