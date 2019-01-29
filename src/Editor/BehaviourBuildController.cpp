// Editor
#include <Editor/BehaviourBuildController.hpp>
#include <Editor/Application.hpp>
#include <Editor/ProjectController.hpp>

// C++ STL
#include <fstream>

HG::Editor::BehaviourBuildController::BehaviourBuildController(HG::Editor::Application* parent) :
    m_parentApplication(parent)
{

}

void HG::Editor::BehaviourBuildController::createConfigurationFile(const std::filesystem::path& path,
                                                                   HG::Editor::BehaviourBuildController::ConfigurationFileType fileType)
{
    switch (fileType)
    {
    case ConfigurationFileType::CMakeLists:
        createCMakeLists(path);
        break;
    }
}

void HG::Editor::BehaviourBuildController::configureProject(const std::filesystem::path &configurationFilePath,
                                                            HG::Editor::BehaviourBuildController::ConfigurationFileType fileType,
                                                            const std::filesystem::path &buildDirectory)
{
    switch (fileType)
    {
    case ConfigurationFileType::CMakeLists:
        configureCMakeLists(configurationFilePath, buildDirectory);
        break;
    }
}

void HG::Editor::BehaviourBuildController::createCMakeLists(const std::filesystem::path &path)
{
    std::ofstream file(path);

    if (!file.is_open())
    {
        throw std::invalid_argument("Can't open file for writing configuration file");
    }

    const std::string& assetsDir = m_parentApplication->projectController()->metadata()->assetsDirectory;
    const std::string& projectName = m_parentApplication->projectController()->metadata()->name;

    file << "cmake_minimum_required(VERSION 3.5)" << std::endl;
    file << "project(" << projectName << ")" << std::endl;
    file << std::endl;
    file << "set(ASSETS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/" << assetsDir << ")" << std::endl;
    file << std::endl;
    file << "file(GLOB_RECURSE SOURCES "
            "${ASSETS_DIRECTORY}/*.cpp "
            "${ASSETS_DIRECTORY}/*.c "
            "${ASSETS_DIRECTORY}/*.hpp "
            "${ASSETS_DIRECTORY}/*.h"
            ")" << std::endl;

    file << "add_library(" << projectName << " SHARED ${SOURCES})" << std::endl;

    // todo: Change this behaviour
    std::string enginePath = "/home/ushanovalex/Development/Projects/CPP/HGEngineReloadedImGui/engine/";

    file << "target_include_directories(" << projectName << " PUBLIC ${ASSETS_DIRECTORY})" << std::endl;

    file << "add_subdirectory(" << enginePath << " engine_output_dir)" << std::endl;

    file << "target_link_libraries(" << projectName << " "
            "HGCore "
            "HGRenderingBase "
            "HGRenderingOGL "
            "HGStandard "
            "HGUtils"
            ")" << std::endl;
}

void HG::Editor::BehaviourBuildController::configureCMakeLists(const std::filesystem::path &path,
                                                               const std::filesystem::path &buildDirectory)
{
    // Creation of build directory
    std::error_code errorCode;
    std::filesystem::create_directories(buildDirectory, errorCode);

    if (errorCode)
    {
        throw std::invalid_argument("Can't create build directory: " + errorCode.message());
    }

    std::string command;

    command += "cmake -DHGRenderingOGL_USE_GLFW=On ";
    command += "-DCMAKE_PREFIX_PATH=/home/ushanovalex/Development/Libraries/glew-2.1.0 ";
    command += "-DGLFW_LOCATION=/home/ushanovalex/Development/Libraries/glfw-3.2.1 ";
    command += "-DCMAKE_EXPORT_COMPILE_COMMANDS=On ";
    command += "-B\"";
    command += buildDirectory;
    command += "\" -H\"";
    command += path.parent_path();
    command += "\"";

    // todo: Remove this fucking awful shit
    system(command.c_str());
}
