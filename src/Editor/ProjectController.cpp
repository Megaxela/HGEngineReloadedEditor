// Editor
#include <Editor/ProjectController.hpp>
#include <AssetSystem/AssetsManager.hpp>

HG::Editor::ProjectController::ProjectController(HG::Editor::Application *parent) :
    m_parentApplication(parent),
    m_assetsManager(new HG::AssetSystem::AssetsManager())
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

}

HG::AssetSystem::AssetsManager *HG::Editor::ProjectController::assetManager()
{
    return m_assetsManager;
}
