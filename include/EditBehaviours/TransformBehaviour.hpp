#pragma once

// Editor
#include <Guizmo/Guizmo.hpp>

// HG::Core
#include <HG/Core/Behaviour.hpp>

// HG::Utils
#include <HG/Utils/Rect.hpp>

// glm
#include <glm/vec3.hpp>

namespace HG::Editor::Behaviours
{
    /**
     * @brief Behaviour class, that performs
     * movement of gameobject with ImGuizmo
     */
    class TransformBehaviour : public HG::Core::Behaviour
    {
    public:

        struct Configuration
        {
            HG::Utils::Rect sceneWidgetRect;

            ImGuizmo::OPERATION currentOperation = ImGuizmo::OPERATION::TRANSLATE;
            ImGuizmo::MODE      currentMode      = ImGuizmo::MODE::LOCAL;

            bool useSnap = false;

            glm::vec3 translationSnap = glm::vec3(1.0f, 1.0f, 1.0f);
            float     scaleSnap       = 1.0f;
            float     rotation        = 45.0f;
        };

        /**
         * @brief Constructor.
         */
        TransformBehaviour();

        /**
         * @brief Method for setting selected GameObject.
         * @param gameObject Pointer to game object.
         */
        void setSelectedGameObject(HG::Core::GameObject* gameObject);

        /**
         * @brief Method for getting selected GameObject.
         * @return Pointer to game object.
         */
        HG::Core::GameObject* selectedGameObject() const;

        /**
         * @brief Method for getting configuration of
         * transform behaviour.
         * @return Pointer to configuration.
         */
        Configuration* configuration();

    protected:

        void onUpdate() override;

    private:
        Configuration m_configuration;

        HG::Core::GameObject* m_selectedGameObject;
    };
}


