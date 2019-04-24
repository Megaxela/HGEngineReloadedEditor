// Editor
#include <AssetSystem/Assets/DirectoryAsset.hpp>

#include "AssetSystem/Assets/DirectoryAsset.hpp"
#include <AssetSystem/AssetsManager.hpp>
#include <Editor/Application.hpp>
#include <Tools/GlobalThumbnails.hpp>

HG::Editor::AssetSystem::Assets::DirectoryAsset::DirectoryAsset(std::filesystem::path path) :
    HG::Editor::AssetSystem::Assets::AbstractAsset(std::move(path), DirectoryAsset::AssetId)
{

}

HG::Editor::ThumbnailsCache::Handle HG::Editor::AssetSystem::Assets::DirectoryAsset::icon() const
{
    return assetsManager()
        ->application()
        ->globalThumbnails()
        ->getHandle(HG::Editor::GlobalThumbnails::Thumbs::DirectoryIcon);
}

bool HG::Editor::AssetSystem::Assets::DirectoryAsset::isLoadingRequired() const
{
    return false;
}
