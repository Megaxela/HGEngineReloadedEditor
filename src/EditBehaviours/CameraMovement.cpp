// Editor
#include <EditBehaviours/CameraMovement.hpp>
#include <EditBehaviours/GraphicsInterface.hpp>
#include <Widgets/Scene.hpp>

// HG::Core
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Input.hpp>
#include <HG/Core/Transform.hpp>
#include <HG/Core/Scene.hpp>
#include <HG/Core/Application.hpp>
#include <HG/Core/TimeStatistics.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Camera.hpp>

HG::Editor::Behaviours::CameraMovement::CameraMovement() :
    m_sceneWidget(nullptr),
    m_camera(nullptr),
    m_first(true),
    m_lastMousePosition(),
    m_front(),
    m_yaw(0.0f),
    m_pitch(0.0f),
    m_sensitivity(3.0f),
    m_movementSpeed(2.0f)
{

}

HG::Editor::Behaviours::CameraMovement::~CameraMovement()
{
    m_sceneWidget = nullptr;
}

void HG::Editor::Behaviours::CameraMovement::onUpdate()
{
    auto mousePosition = input()->mouse()->getMousePosition();

    auto isButtonPressed = input()->mouse()->isPressed(HG::Core::Input::Mouse::Buttons::Right);

    if (m_first && isButtonPressed)
    {
        if (!m_sceneWidget->isGlobalPosInScene(mousePosition))
        {
            return;
        }

        m_first = false;
        m_lastMousePosition = input()->mouse()->getMousePosition();
    }

    if (isButtonPressed)
    {
        handleMouseRotation();

        handleKeyboardMovement();
    }
    else
    {
        m_first = true;
    }
}

void HG::Editor::Behaviours::CameraMovement::handleMouseRotation()
{
// Frame sens
    float frameSensitivity =
        m_sensitivity / 10;

    // Mouse
    auto mousePosition = input()->mouse()->getMousePosition();
    mousePosition -= m_lastMousePosition;

    m_yaw += mousePosition.x * frameSensitivity;
    m_pitch += mousePosition.y * frameSensitivity;

    m_lastMousePosition = input()->mouse()->getMousePosition();

    if (m_pitch >= 89.0f)
    {
        m_pitch = 89.0f;
    }

    if (m_pitch <= -89.0f)
    {
        m_pitch = -89.0f;
    }

    m_front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    m_front.y = std::sin(glm::radians(m_pitch));
    m_front.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    m_front = glm::normalize(m_front);

    m_camera->lookAt(
        m_camera->gameObject()->transform()->localPosition() + m_front
    );
}

void HG::Editor::Behaviours::CameraMovement::handleKeyboardMovement()
{
    auto dt = scene()
                  ->application()
                  ->timeStatistics()
                  ->lastFrameDeltaTime().count() / 1000000.0;

    auto input = scene()->application()->input()->keyboard();

    glm::vec3 inputDirection(0.0f, 0.0f, 0.0f);

    auto speed = static_cast<float>(m_movementSpeed * dt);

    if (input->isPressed(HG::Core::Input::Keyboard::Key::Q))
    {
        inputDirection.y -= speed;
    }

    if (input->isPressed(HG::Core::Input::Keyboard::Key::E))
    {
        inputDirection.y += speed;
    }

    if (input->isPressed(HG::Core::Input::Keyboard::Key::W))
    {
        inputDirection.z -= speed;
    }

    if (input->isPressed(HG::Core::Input::Keyboard::Key::S))
    {
        inputDirection.z += speed;
    }

    if (input->isPressed(HG::Core::Input::Keyboard::Key::A))
    {
        inputDirection.x -= speed;
    }

    if (input->isPressed(HG::Core::Input::Keyboard::Key::D))
    {
        inputDirection.x += speed;
    }

    if (input->isPressed(HG::Core::Input::Keyboard::Key::F))
    {
        if (m_camera->projection() == HG::Rendering::Base::Camera::Projection::Orthogonal)
        {
            m_camera->setProjection(HG::Rendering::Base::Camera::Projection::Perspective);
        }
        else
        {
            m_camera->setProjection(HG::Rendering::Base::Camera::Projection::Orthogonal);
        }
    }

    gameObject()->transform()->setGlobalPosition(
        gameObject()->transform()->globalPosition() +
        inputDirection * m_camera->gameObject()->transform()->globalRotation()
    );
}

void HG::Editor::Behaviours::CameraMovement::onStart()
{
    auto* graphicsInterface = gameObject()->findBehaviour<HG::Editor::Behaviours::GraphicsInterface>();

    if (graphicsInterface == nullptr)
    {
        throw std::runtime_error("No graphics interface for camera movement behaviour");
    }

    m_sceneWidget = graphicsInterface->sceneWidget();

    m_camera = gameObject()->findBehaviour<HG::Rendering::Base::Camera>();

    if (m_camera == nullptr)
    {
        throw std::runtime_error("No camera for camera movement behaviour");
    }
}
