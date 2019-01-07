// Editor
#include <EditBehaviours/TransformBehaviour.hpp>

// HG::Core
#include <HG/Core/Scene.hpp>
#include <HG/Core/GameObject.hpp>
#include <HG/Core/Transform.hpp>
#include <HG/Core/Application.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/Camera.hpp>

// ImGui
#include <imgui.h>

// ALogger
#include <CurrentLogger.hpp>

HG::Editor::Behaviours::TransformBehaviour::TransformBehaviour() :
    m_configuration(),
    m_selectedGameObject(nullptr)
{

}

void HG::Editor::Behaviours::TransformBehaviour::setSelectedGameObject(HG::Core::GameObject* gameObject)
{
    m_selectedGameObject = gameObject;
}

HG::Core::GameObject* HG::Editor::Behaviours::TransformBehaviour::selectedGameObject() const
{
    return m_selectedGameObject;
}

HG::Editor::Behaviours::TransformBehaviour::Configuration* HG::Editor::Behaviours::TransformBehaviour::configuration()
{
    return &m_configuration;
}

void HG::Editor::Behaviours::TransformBehaviour::onUpdate()
{
    if (m_selectedGameObject == nullptr)
    {
        return;
    }

    auto matrix = m_selectedGameObject->transform()->localToWorldMatrix();

    float* snapping = nullptr;

    if (m_configuration.useSnap)
    {
        switch (m_configuration.currentOperation)
        {
        case ImGuizmo::TRANSLATE:
            snapping = &m_configuration.translationSnap.x;
            break;
        case ImGuizmo::ROTATE:
            snapping = &m_configuration.rotation;
            break;
        case ImGuizmo::SCALE:
            snapping = &m_configuration.scaleSnap;
            break;
        case ImGuizmo::BOUNDS:
            break;
        }
    }

    // Camera position
    auto cameraMatrix =
        scene()->application()->renderer()->activeCamera()
            ->viewMatrix();

    // Projection matrix
    auto projMatrix =
        scene()->application()->renderer()->activeCamera()
            ->projectionMatrix();

    ImGuiIO& io = ImGui::GetIO();
//    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    ImGuizmo::SetRect(
        m_configuration.sceneWidgetRect.x,
        m_configuration.sceneWidgetRect.y,
        m_configuration.sceneWidgetRect.w,
        m_configuration.sceneWidgetRect.h
    );

    ImGuizmo::Manipulate(
        reinterpret_cast<const float*>(&cameraMatrix),
        reinterpret_cast<const float*>(&projMatrix),
        m_configuration.currentOperation,
        m_configuration.currentMode,
        reinterpret_cast<float*>(&matrix),
        nullptr,
        snapping
    );

    m_selectedGameObject->transform()->setFromLocalToWorldMatrix(matrix);
}
