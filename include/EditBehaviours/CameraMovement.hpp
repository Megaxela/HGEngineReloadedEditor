#pragma once

// HG::Core
#include <HG/Core/Behaviour.hpp>

// GLM
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace HG::Rendering::Base
{
    class Camera;
}

namespace HG::Editor::Widgets
{
    class Scene;
}

namespace HG::Editor::Behaviours
{
    /**
     * @brief Behaviour class, that implements editors
     * camera movement.
     */
    class CameraMovement : public HG::Core::Behaviour
    {
    public:

        /**
         * @brief Constructor.
         */
        CameraMovement();

        /**
         * @brief Destructor.
         */
        ~CameraMovement() override;

    protected:

        /**
         * @brief Method, that's called every frame and manages
         * user input for camera movement.
         */
        void onUpdate() override;

        /**
         * @brief Method, that's called once at start.
         * It has to identify other behaviours.
         */
        void onStart() override;

    private:

        void handleMouseRotation();

        void handleKeyboardMovement();

        HG::Editor::Widgets::Scene* m_sceneWidget;

        HG::Rendering::Base::Camera* m_camera;

        bool m_first;

        glm::ivec2 m_lastMousePosition;
        glm::vec3 m_front;
        float m_yaw;
        float m_pitch;

        float m_sensitivity;
        float m_movementSpeed;
    };
}


