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
        reloadAssets();
    }

    // If assets loading is preformed and all assets are loaded
    // rebuild thumbnails cache and call post load.
    if (m_isLoading &&
        m_pendingLoadingAssets == 0)
    {
        // Invalidate cache, cause assets may create
        // their thumbnails
        if (application()->thumbnailsCache()->invalidationRequired())
        {
            application()->thumbnailsCache()->invalidateCache();
        }

        // Performing assets `postLoad` method
        performAssetsPostLoad(m_rootAsset);

        // Run building if required


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
    // If there is no loaded assets
    if (m_rootAsset == nullptr)
    {
        m_rootAsset = std::make_shared<HG::Editor::AssetSystem::Assets::RootAsset>(m_assetsPath);
        return;
    }

    // If there is some loaded assets - update them
    reloadDirectory(m_assetsPath, m_rootAsset);
    postAssetForLoading(m_rootAsset);

    m_isLoading = true;
}

void HG::Editor::AssetSystem::AssetsManager::reloadAssets()
{
    m_rootAsset = std::make_shared<HG::Editor::AssetSystem::Assets::RootAsset>(m_assetsPath);

    reloadDirectory(m_assetsPath, m_rootAsset);
    postAssetForLoading(m_rootAsset);

    m_isLoading = true;
}

void HG::Editor::AssetSystem::AssetsManager::reloadDirectory(const std::filesystem::path &path, HG::Editor::AssetSystem::Assets::AssetPtr target)
{
    // todo: Add to watching here
    // Checking for deleted assets
    removeDeletedAssets(target);

    for (auto&& iterator : std::filesystem::directory_iterator(path))
    {
        // Checking was asset already loaded?
        // If it was - checking it's type and invalidating it
        auto childAssetIterator = std::find_if(
            target->children().begin(),
            target->children().end(),
            [&iterator](const HG::Editor::AssetSystem::Assets::AssetPtr& asset)
            {
                return asset->path() == iterator;
            }
        );

        HG::Editor::AssetSystem::Assets::AssetPtr asset;

        // todo: Add type check with fabric
        if (childAssetIterator != target->children().end())
        {
            asset = (*childAssetIterator);

            Debug() << "Invalidating asset \"" << asset->path() << "\".";
            asset->invalidate();
        }
        else
        {
            asset = application()->assetsFabric()->create(iterator);
            asset->setAssetsManager(this);
            target->addChild(asset);
        }

        if (asset->type() == HG::Editor::AssetSystem::Assets::DirectoryAsset::AssetId)
        {
            reloadDirectory(iterator, asset);
        }
    }
}

void HG::Editor::AssetSystem::AssetsManager::removeDeletedAssets(HG::Editor::AssetSystem::Assets::AssetPtr rootAsset)
{
    std::vector<HG::Editor::AssetSystem::Assets::AssetPtr> assetsToRemove;

    // todo: Add type check with fabric
    for (const auto& asset : rootAsset->children())
    {
        if (!std::filesystem::exists(asset->path()))
        {
            assetsToRemove.push_back(asset);
            continue;
        }

        removeDeletedAssets(asset);
    }

    for (const auto& asset : assetsToRemove)
    {
        rootAsset->children().erase(
            std::remove(
                rootAsset->children().begin(),
                rootAsset->children().end(),
                asset
            ),
            rootAsset->children().end()
        );
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

    if (asset->state() == Editor::AssetSystem::Assets::AbstractAsset::State::NotLoaded ||
        asset->state() == Editor::AssetSystem::Assets::AbstractAsset::State::Failed)
    {
        m_pendingLoadingAssets += 1;

        m_parentApplication->threadPool()->push(
                [asset, this]()
                {
                    asset->load();

                    --m_pendingLoadingAssets;
                },
                HG::Core::ThreadPool::Type::UserThread
        );
    }

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
