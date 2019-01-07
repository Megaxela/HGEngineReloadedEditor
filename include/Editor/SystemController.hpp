#pragma once

// todo: Remove renderer specific includes and make this class templated
// HG::Core::Rendering::OpenGL
#include <HG/Rendering/OpenGL/GLFWSystemController.hpp>

namespace HG::Editor
{
    /**
     * @brief Class, that describes custom system controller
     * for editor.
     */
    class SystemController : public HG::Rendering::OpenGL::GLFWSystemController
    {
    public:
        explicit SystemController(HG::Core::Application* application);

        void pollEvents() override;
    };
}


