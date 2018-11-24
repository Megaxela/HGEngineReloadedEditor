// Editor
#include <Editor/ProjectController.hpp>
#include <AssetSystem/AssetsManager.hpp>

// C++ STL
#include <fstream>

// ALogger
#include <CurrentLogger.hpp>

HG::Editor::ProjectController::ProjectController(HG::Editor::Application *parent) :
    m_parentApplication(parent),
    m_assetsManager(new HG::AssetSystem::AssetsManager(parent))
{

}

HG::Editor::ProjectController::~ProjectController()
{
    delete m_assetsManager;
}

HG::Editor::Application *HG::Editor::ProjectController::application() const
{
    return m_parentApplication;
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

    parseMetadata(projectJson["metadata"]);

    m_assetsManager->setAssetsPath(std::move(path / m_metadata.assetsDirectory));
}

HG::AssetSystem::AssetsManager *HG::Editor::ProjectController::assetManager()
{
    return m_assetsManager;
}

void HG::Editor::ProjectController::parseMetadata(nlohmann::json metadataJson)
{
    m_metadata.assetsDirectory = metadataJson["assets_directory"];
    m_metadata.activeScene = metadataJson["active_scene"];
}
