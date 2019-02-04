// Editor
#include <Editor/BehaviourBuildController.hpp>
#include <Editor/Application.hpp>
#include <Editor/ProjectController.hpp>
#include <Editor/EngineInfo.hpp>

// C++ STL
#include <fstream>
#include <iostream>

// libexecstream
#include <exec-stream.h>

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

bool HG::Editor::BehaviourBuildController::configureProject(const std::filesystem::path &configurationFilePath,
                                                            HG::Editor::BehaviourBuildController::ConfigurationFileType fileType,
                                                            const std::filesystem::path &buildDirectory)
{
    switch (fileType)
    {
    case ConfigurationFileType::CMakeLists:
        return configureCMakeLists(configurationFilePath, buildDirectory);
    }

    return false;
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

    auto enginePath = std::filesystem::current_path() / "engine";

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

    file << "target_include_directories(" << projectName
         << " PUBLIC ${ASSETS_DIRECTORY} "
         << (enginePath / "include") << ")" << std::endl;

    file << "target_link_libraries(" << projectName << " ";

    auto modules = m_parentApplication
        ->engineInfo()
        ->getAvailableModules();

    for (auto& moduleName : modules)
    {
        auto moduleInfo = m_parentApplication
            ->engineInfo()
            ->getModuleInfo(moduleName);

        file << (enginePath / "lib" / moduleInfo.libraryName) << " " << std::endl;
    }

    file << ")" << std::endl;

    file << "set_target_properties(" << projectName << ' ' <<
            "PROPERTIES CXX_STANDARD 17"
            ")";
}

bool HG::Editor::BehaviourBuildController::configureCMakeLists(const std::filesystem::path &path,
                                                               const std::filesystem::path &buildDirectory)
{
    // Creation of build directory
    std::error_code errorCode;
    std::filesystem::create_directories(buildDirectory, errorCode);

    if (errorCode)
    {
        throw std::invalid_argument("Can't create build directory: " + errorCode.message());
    }

    std::vector<std::string> arguments = {
        "-DCMAKE_PREFIX_PATH=/home/ushanovalex/Development/Libraries/glew-2.1.0",
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=On",
        "-DCMAKE_CXX_COMPILER=clang++-6.0",
        "-DCMAKE_C_COMPILER=clang-6.0",
        "-B" + buildDirectory.string(),
        "-H" + path.parent_path().string()
    };

    for (auto& el : arguments)
    {
        std::cout << el << std::endl;
    }

    exec_stream_t command(
        "cmake", // todo: Make this path configurable
        arguments.begin(),
        arguments.end()
    );

    std::string line;

    while (std::getline(command.out(), line))
    {
        std::cout << line << std::endl;
    }

    command.close();

    return command.exit_code() == 0;
}
