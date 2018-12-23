// Editor
#include <AssetSystem/AssetsManager.hpp>
#include <Editor/Application.hpp>
#include <AssetSystem/Assets/RootAsset.hpp>
#include <Fabrics/AssetsFabric.hpp>

// HG::Core
#include <HG/Core/ThreadPool.hpp>

// ALogger
#include <CurrentLogger.hpp>
#include <AssetSystem/Assets/DirectoryAsset.hpp>

HG::Editor::AssetSystem::AssetsManager::AssetsManager(HG::Editor::Application *parent) :
    m_assetsPathChanged(false),
    m_assetsPath(),
    m_rootAsset(nullptr),
    m_parentApplication(parent),
    m_pendingLoadingAssets(0),
    m_isLoading(false)
{

}

HG::Editor::Application *HG::Editor::AssetSystem::AssetsManager::application() const
{
    return m_parentApplication;
}

void HG::Editor::AssetSystem::AssetsManager::proceedEvents()
{
    if (m_assetsPathChanged)
    {
        m_assetsPathChanged = false;

        Info() << "Assets path changed. Reloading assets.";

        clearAssets();
        updateAssets();
    }

    // If assets loading is preformed and all assets are loaded
    // rebuild thumbnails cache and call post load.
    if (m_isLoading &&
        m_pendingLoadingAssets == 0)
    {
        // Invalidate cache, cause assets may create
        // their thumbnails
        application()->thumbnailsCache()->invalidateCache();

        // Performing assets `postLoad` method
        performAssetsPostLoad(m_rootAsset);

        // Finishing assets loading
        m_isLoading = false;
    }
}

void HG::Editor::AssetSystem::AssetsManager::setAssetsPath(std::filesystem::path path)
{
    if (m_assetsPath != path)
    {
        m_assetsPathChanged = true;
        m_assetsPath = std::move(path);
    }
}

std::filesystem::path HG::Editor::AssetSystem::AssetsManager::assetsPath() const
{
    return m_assetsPath;
}

void HG::Editor::AssetSystem::AssetsManager::clearAssets()
{
    m_rootAsset = nullptr;
}

void HG::Editor::AssetSystem::AssetsManager::updateAssets()
{
    if (m_rootAsset == nullptr)
    {
        reloadAssets();
        postAssetsForLoading();
        return;
    }
}

void HG::Editor::AssetSystem::AssetsManager::reloadAssets()
{
    m_rootAsset = std::make_shared<HG::Editor::AssetSystem::Assets::RootAsset>(m_assetsPath);

    reloadDirectory(m_assetsPath, m_rootAsset);
}

void HG::Editor::AssetSystem::AssetsManager::reloadDirectory(const std::filesystem::path &path, HG::Editor::AssetSystem::Assets::AssetPtr target)
{
    // todo: Add to watching here

    for (auto&& iterator : std::filesystem::directory_iterator(path))
    {
        auto newAsset = application()->assetsFabric()->create(iterator);
        newAsset->setAssetsManager(this);

        if (newAsset->type() == HG::Editor::AssetSystem::Assets::DirectoryAsset::AssetId)
        {
            reloadDirectory(iterator, newAsset);
        }

        target->addChild(newAsset);
    }
}

HG::Editor::AssetSystem::Assets::AssetPtr HG::Editor::AssetSystem::AssetsManager::rootAsset() const
{
    return m_rootAsset;
}

void HG::Editor::AssetSystem::AssetsManager::postAssetsForLoading()
{
    postAssetForLoading(m_rootAsset);

    m_isLoading = true;
}

void HG::Editor::AssetSystem::AssetsManager::postAssetForLoading(HG::Editor::AssetSystem::Assets::AssetPtr asset)
{
    if (asset == nullptr ||
        m_parentApplication == nullptr)
    {
        throw std::runtime_error("Asset or parent application is null");
    }

    m_pendingLoadingAssets += 1;

    m_parentApplication->threadPool()->push(
        [asset, this]()
        {
            asset->load();

            --m_pendingLoadingAssets;
        },
        HG::Core::ThreadPool::Type::UserThread
    );

    for (auto& child : asset->children())
    {
        postAssetForLoading(child);
    }
}

void HG::Editor::AssetSystem::AssetsManager::performAssetsPostLoad(HG::Editor::AssetSystem::Assets::AssetPtr asset)
{
    if (asset == nullptr ||
        m_parentApplication == nullptr)
    {
        throw std::runtime_error("Asset or parent application is null");
    }

    asset->postLoad();

    for (auto& child : asset->children())
    {
        performAssetsPostLoad(child);
    }
}
