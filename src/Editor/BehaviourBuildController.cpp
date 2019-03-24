// Editor
#include <Editor/BehaviourBuildController.hpp>
#include <Editor/Application.hpp>
#include <Editor/ProjectController.hpp>
#include <Editor/EngineInfo.hpp>

// HG::Utils
#include <HG/Utils/StringTools.hpp>

// C++ STL
#include <fstream>
#include <iostream>

// libexecstream
#include <exec-stream.h>
#include <CurrentLogger.hpp>

HG::Editor::BehaviourBuildController::BehaviourBuildController(HG::Editor::Application* parent) :
    m_parentApplication(parent),
    m_loadedFileType(),
    m_compilationDBPath(),
    m_buildDirectory(),
    m_database()
{

}

void HG::Editor::BehaviourBuildController::createConfigurationFile(const std::filesystem::path& path,
                                                                   HG::Editor::BehaviourBuildController::ConfigurationFileType fileType)
{
    switch (fileType)
    {
    case ConfigurationFileType::CMakeLists:
        createCMakeLists(path);
        return;
    }

    throw std::invalid_argument("Unknown configuration file type");
}

bool HG::Editor::BehaviourBuildController::configureProject(const std::filesystem::path &configurationFilePath,
                                                            HG::Editor::BehaviourBuildController::ConfigurationFileType fileType,
                                                            const std::filesystem::path &buildDirectory)
{
    bool configurationStatus = false;
    switch (fileType)
    {
    case ConfigurationFileType::CMakeLists:
        configurationStatus = configureCMakeLists(configurationFilePath, buildDirectory);
        break;
    }

    if (!configurationStatus)
    {
        return false;
    }

    if (!createLibraryLoader(buildDirectory))
    {
        return false;
    }

    m_buildDirectory = buildDirectory;

    return loadBuildInfoFrom(buildDirectory / "compile_commands.json", fileType);
}

void HG::Editor::BehaviourBuildController::createCMakeLists(const std::filesystem::path &path)
{
    auto pathToProject = path.parent_path();
    auto cmakeDirectoryPath = pathToProject / "cmake";

    // Copying ConfigureProject.cmake
    std::error_code errorCode;
    std::filesystem::create_directories(pathToProject / "cmake", errorCode);

    if (errorCode)
    {
        throw std::invalid_argument("Can't create cmake directory: " + errorCode.message());
    }

    const std::string fileName = "ConfigureProject.cmake";

    auto pathToEngineConfigure = std::filesystem::current_path() / "project_files" / fileName;

    if (!std::filesystem::exists(pathToEngineConfigure))
    {
        throw std::invalid_argument("Can't copy \"" + pathToEngineConfigure.string() + "\". Error: File does not exists");
    }

    // todo: remove skip_existing when project version will be implemented
    std::filesystem::copy(
        pathToEngineConfigure,
        cmakeDirectoryPath / fileName,
        std::filesystem::copy_options::skip_existing,
        errorCode
    );

    if (errorCode)
    {
        throw std::invalid_argument("Can't copy \"" + fileName + "\". Error: " + errorCode.message());
    }

    std::ofstream file(path);

    if (!file.is_open())
    {
        throw std::invalid_argument("Can't open configuration file for writing");
    }

    const std::string& assetsDir = m_parentApplication->projectController()->metadata()->assetsDirectory;
    std::string projectName = m_parentApplication->projectController()->metadata()->name;

    // Replacing spaces with _
    std::replace(
        projectName.begin(),
        projectName.end(),
        ' ', '_'
    );

    auto enginePath = std::filesystem::current_path() / "engine";

    file << "cmake_minimum_required(VERSION 3.13)" << std::endl
         << std::endl
         << "include(cmake/ConfigureProject.cmake)" << std::endl
         << std::endl
         << "prepare_project(" << std::endl
         << "    PROJECT_NAME " << projectName << std::endl
         << "    ASSETS_DIRECTORY " << assetsDir << std::endl
         << "    PATH_TO_ENGINE " << enginePath << std::endl
         << "    DEPENDENCIES " << std::endl;

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
}

bool HG::Editor::BehaviourBuildController::configureCMakeLists(const std::filesystem::path &path,
                                                               const std::filesystem::path &buildDirectory)
{
    auto pathToProject = path.parent_path();

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
//        "-DCMAKE_CXX_COMPILER=/usr/bin/clang++-6.0",
//        "-DCMAKE_C_COMPILER=/usr/bin/clang-6.0",
        "-B" + buildDirectory.string(),
        "-H" + pathToProject.string()
    };

    for (auto& el : arguments)
    {
        std::cout << el << std::endl;
    }

    exec_stream_t command(
        "/bin/cmake", // todo: Make this path configurable
        arguments.begin(),
        arguments.end()
    );

    std::string line;

    while (std::getline(command.err(), line))
    {
        std::cout << line << std::endl;
    }

    command.close();

    return command.exit_code() == 0;
}

HG::Editor::BehaviourBuildController::UnitBuildInfo HG::Editor::BehaviourBuildController::getBuildInfoFor(const std::filesystem::path &file) const
{
    auto absolutePath = std::filesystem::absolute(file);

    auto searchResult = m_database.find(absolutePath);

    if (searchResult == m_database.end())
    {
        throw std::invalid_argument("There is no compilation database at \"" + file.string() + "\"");
    }

    return searchResult->second;
}

bool HG::Editor::BehaviourBuildController::loadBuildInfoFrom(const std::filesystem::path &compilationDatabasePath,
                                                             HG::Editor::BehaviourBuildController::ConfigurationFileType fileType)
{
    m_loadedFileType = fileType;
    switch (fileType)
    {
    case ConfigurationFileType::CMakeLists:
        return loadCMakeListsBuildInfoFrom(compilationDatabasePath);
    }

    throw std::invalid_argument("Unknown configuration file type");
}

bool HG::Editor::BehaviourBuildController::loadCMakeListsBuildInfoFrom(const std::filesystem::path &compilationDatabasePath)
{
    std::ifstream file(compilationDatabasePath);

    // No cpp assets found, no compilation database though
    if (!file.is_open())
    {
        return true;
    }

    m_database.clear();

    nlohmann::json data = nlohmann::json::parse(file);

    for (auto& entry : data)
    {
        UnitBuildInfo info;
        std::string command;

        entry.at("directory").get_to(info.directory);
        entry.at("command").get_to(command);
        entry.at("file").get_to(info.file);

        parseCLICommand(command, info.command, info.arguments);

        m_database.insert({
            std::filesystem::absolute(info.file),
            info
        });
    }

    return true;
}

constexpr const char* libraryTUCode = R"(
namespace HG::Core
{
    class Behaviour;
}

extern "C" HG::Core::Behaviour* createBehaviour()
{
    return new BEHAVIOUR_NAME();
}

extern "C" void deleteBehaviour(HG::Core::Behaviour* behaviour)
{
    delete behaviour;
}
)";

bool HG::Editor::BehaviourBuildController::createLibraryLoader(const std::filesystem::path &path)
{
    std::ofstream file(path / "library_code.cpp");

    if (!file.is_open())
    {
        return false;
    }

    file << libraryTUCode;

    return true;
}

std::filesystem::path HG::Editor::BehaviourBuildController::buildSource(const std::filesystem::path &path)
{
    auto buildInfo = getBuildInfoFor(path);

    std::vector<std::string> buildArguments;

    for (auto& argument : buildInfo.arguments)
    {
        if (HG::Utils::StringTools::startsWith<std::string>(argument, "-o"))
        {
            continue;
        }

        buildArguments.push_back(argument);
    }

    auto pathToFile = path.string() + ".o";
    buildArguments.push_back(pathToFile);

    exec_stream_t builder(
        buildInfo.command,
        buildArguments.begin(),
        buildArguments.end()
    );

    std::string line;
    while (std::getline(builder.err(), line))
    {
        Error() << "Compilation error: " << line;
    }

    builder.close();

    return pathToFile;
}

void HG::Editor::BehaviourBuildController::parseCLICommand(const std::string& cliCommand, std::string &command, std::vector<std::string> &arguments)
{
    auto splittedCommand = HG::Utils::StringTools::split(cliCommand, ' ');

    if (splittedCommand.empty())
    {
        return;
    }

    auto iter = splittedCommand.begin();

    command = *(iter++);

    for (; iter != splittedCommand.end(); ++iter)
    {
        arguments.emplace_back(std::move(*iter));
    }
}
