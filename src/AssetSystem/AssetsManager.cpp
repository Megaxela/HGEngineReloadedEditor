// Editor
#include <AssetSystem/AssetsManager.hpp>
#include <Editor/Application.hpp>
#include <AssetSystem/Assets/RootAsset.hpp>
#include <Fabrics/AssetsFabric.hpp>

// ALogger
#include <CurrentLogger.hpp>
#include <AssetSystem/Assets/DirectoryAsset.hpp>

HG::AssetSystem::AssetsManager::AssetsManager(HG::Editor::Application *parent) :
    m_assetsPathChanged(false),
    m_assetsPath(),
    m_rootAsset(nullptr),
    m_parentApplication(parent)
{

}

HG::Editor::Application *HG::AssetSystem::AssetsManager::application() const
{
    return m_parentApplication;
}

void HG::AssetSystem::AssetsManager::proceedEvents()
{
    if (m_assetsPathChanged)
    {
        m_assetsPathChanged = false;

        Info() << "Assets path changed. Reloading assets.";

        clearAssets();
        updateAssets();
    }
}

void HG::AssetSystem::AssetsManager::setAssetsPath(std::filesystem::path path)
{
    if (m_assetsPath != path)
    {
        m_assetsPathChanged = true;
        m_assetsPath = std::move(path);
    }
}

std::filesystem::path HG::AssetSystem::AssetsManager::assetsPath() const
{
    return m_assetsPath;
}

void HG::AssetSystem::AssetsManager::clearAssets()
{
    m_rootAsset = nullptr;
}

void HG::AssetSystem::AssetsManager::updateAssets()
{
    if (m_rootAsset == nullptr)
    {
        reloadAssets();
        return;
    }
}

void HG::AssetSystem::AssetsManager::reloadAssets()
{
    m_rootAsset = std::make_shared<HG::Editor::AssetSystem::Assets::RootAsset>(m_assetsPath);

    reloadDirectory(m_assetsPath, m_rootAsset);
}

void HG::AssetSystem::AssetsManager::reloadDirectory(const std::filesystem::path &path, HG::Editor::AssetSystem::Assets::AssetPtr target)
{
    // todo: Add to watching here

    for (auto&& iterator : std::filesystem::directory_iterator(path))
    {
        auto newAsset = application()->assetsFabric()->create(iterator);

        if (newAsset->type() == HG::Editor::AssetSystem::Assets::DirectoryAsset::AssetId)
        {
            reloadDirectory(iterator, newAsset);
        }

        target->addChild(newAsset);
    }
}

HG::Editor::AssetSystem::Assets::AssetPtr HG::AssetSystem::AssetsManager::rootAsset() const
{
    return m_rootAsset;
}
