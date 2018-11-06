#include <AssetSystem/AssetsManager.hpp>

HG::AssetSystem::AssetsManager::AssetsManager(HG::Editor::Application *parent) :
    m_assetsPath(),
    m_rootAssets(),
    m_parentApplication(parent)
{

}

HG::Editor::Application *HG::AssetSystem::AssetsManager::application() const
{
    return m_parentApplication;
}

void HG::AssetSystem::AssetsManager::proceedEvents()
{

}

void HG::AssetSystem::AssetsManager::setAssetsPath(std::filesystem::path path)
{
    m_assetsPath = std::move(path);
}

std::filesystem::path HG::AssetSystem::AssetsManager::assetsPath() const
{
    return m_assetsPath;
}
