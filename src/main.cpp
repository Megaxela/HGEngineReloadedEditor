// Editor
#include <Scenes/EditorScene.hpp>
#include <Editor/Application.hpp>

// HG::Core
#include <HG/Core/ResourceManager.hpp>

// HG::Standard
#include <HG/Core/FilesystemResourceAccessor.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Renderer.hpp>

// HG::Rendering::OpenGL
#include <HG/Rendering/OpenGL/Forward/RenderingPipeline.hpp>
#include <HG/Rendering/OpenGL/Forward/MeshRenderer.hpp>
#include <HG/Rendering/OpenGL/Common/MeshDataProcessor.hpp>
#include <HG/Rendering/OpenGL/Common/Texture2DDataProcessor.hpp>
#include <HG/Rendering/OpenGL/Common/ShaderDataProcessor.hpp>
#include <HG/Rendering/OpenGL/Common/RenderTargetDataProcessor.hpp>
#include <HG/Rendering/OpenGL/Forward/CubeMapRenderer.hpp>
#include <HG/Rendering/OpenGL/Common/CubeMapTextureDataProcessor.hpp>

// ALogger
#include <CurrentLogger.hpp>
#include <Loggers/BasicLogger.hpp>
#include <Editor/SystemController.hpp>

int main(int argc, char** argv)
{
    CurrentLogger::setCurrentLogger(std::make_shared<Loggers::BasicLogger>());

    HG::Editor::Application application("HGEngine Editor", argc, argv);

    // Setting resource accessor implementation
    application.resourceManager()
        ->setResourceAccessor(new HG::Core::FilesystemResourceAccessor());

    // Setting system controller
    application.setSystemController(new HG::Editor::SystemController(&application));

    auto pipeline = new HG::Rendering::OpenGL::Forward::RenderingPipeline(&application);
    pipeline->addRenderer(new HG::Rendering::OpenGL::Forward::MeshRenderer);
    pipeline->addRenderer(new HG::Rendering::OpenGL::Forward::CubeMapRenderer);
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::RenderTargetDataProcessor);
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::MeshDataProcessor);
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::Texture2DDataProcessor);
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::ShaderDataProcessor);
    pipeline->addRenderDataProcessor(new HG::Rendering::OpenGL::Common::CubeMapTextureDataProcessor);

    // Setting rendering to forward
    application.renderer()->setPipeline(pipeline);

    if (!application.init())
    {
        ErrorF() << "Can't init application.";
        return -1;
    }

    // Enabling docking for ImGui
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    application.setScene(new HG::Editor::EditorScene());

    return application.exec();
}