// Editor
#include <Editor/ProjectController.hpp>
#include <Editor/BehaviourBuildController.hpp>
#include <AssetSystem/AssetsManager.hpp>

// C++ STL
#include <fstream>

// ALogger
#include <CurrentLogger.hpp>

HG::Editor::ProjectController::ProjectController(HG::Editor::Application *parent) :
    m_projectPath(),
    m_parentApplication(parent),
    m_assetsManager(new HG::Editor::AssetSystem::AssetsManager(parent)),
    m_behaviourBuildController(new HG::Editor::BehaviourBuildController(parent))
{

}

HG::Editor::ProjectController::~ProjectController()
{
    delete m_behaviourBuildController;
    delete m_assetsManager;
}

HG::Editor::Application *HG::Editor::ProjectController::application() const
{
    return m_parentApplication;
}


HG::Editor::ProjectMetadata* HG::Editor::ProjectController::metadata()
{
    return &m_metadata;
}

void HG::Editor::ProjectController::load(std::filesystem::path path)
{
    auto projectInfoPath = path / "project_info.json";

    if (!std::filesystem::exists(projectInfoPath))
    {
        throw std::invalid_argument("No project's metadata info in project's directory");
    }

    // Trying to open
    std::ifstream projectFile(projectInfoPath);

    if (!projectFile.is_open())
    {
        throw std::invalid_argument("Can't open project's metadata info");
    }

    nlohmann::json projectJson = nlohmann::json::parse(projectFile);

    projectJson.at("metadata").get_to(m_metadata);

    m_assetsManager->setAssetsPath(std::move(path / m_metadata.assetsDirectory));
}

HG::Editor::AssetSystem::AssetsManager *HG::Editor::ProjectController::assetManager()
{
    return m_assetsManager;
}

HG::Editor::BehaviourBuildController* HG::Editor::ProjectController::behaviourBuildController()
{
    return m_behaviourBuildController;
}

void HG::Editor::ProjectController::save()
{
    if (m_projectPath.empty())
    {
        throw std::runtime_error("Trying to save non opened project.");
    }

    // Writing to `project_info.json`.
    auto projectInfoPath = m_projectPath / "project_info.json";

    // Trying to open
    std::ofstream projectFile(projectInfoPath);

    if (!projectFile.is_open())
    {
        throw std::runtime_error("Can't open project's metadata info for saving");
    }

    nlohmann::json projectData;
    projectData["metadata"] = m_metadata;

    projectFile << projectData;

    // Trying to create directory with assets
    std::filesystem::create_directories(m_projectPath / m_metadata.assetsDirectory);

    // Creating configuration file
    m_behaviourBuildController->createConfigurationFile(
        m_projectPath / "CMakeLists.txt",
        HG::Editor::BehaviourBuildController::ConfigurationFileType::CMakeLists
    );
}

void HG::Editor::ProjectController::create(std::filesystem::path path, std::string name)
{
    // Trying to create directory
    std::error_code errorCode;
    std::filesystem::create_directories(path, errorCode);

    if (errorCode)
    {
        throw std::runtime_error("Can't create directories for new project: " + errorCode.message());
    }

    // Filling project related info
    m_projectPath = std::move(path);
    m_metadata.assetsDirectory = "Assets";
    m_metadata.activeScene = "";
    m_metadata.name = std::move(name);

    // Saving
    save();
}

void HG::Editor::ProjectController::close()
{
    m_projectPath.clear();
    m_metadata.assetsDirectory.clear();
    m_metadata.activeScene.clear();
    m_metadata.name.clear();
}

bool HG::Editor::ProjectController::isOpened() const
{
    return !m_projectPath.empty();
}

std::filesystem::path HG::Editor::ProjectController::projectPath() const
{
    return m_projectPath;
}
