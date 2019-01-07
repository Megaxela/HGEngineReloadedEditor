// Editor
#include <Editor/SystemController.hpp>
#include <Guizmo/Guizmo.hpp>

HG::Editor::SystemController::SystemController(HG::Core::Application* application) :
    GLFWSystemController(application)
{

}

void HG::Editor::SystemController::pollEvents()
{
    GLFWSystemController::pollEvents();
    ImGuizmo::BeginFrame();
}
