// Editor
#include <AssetSystem/Assets/DirectoryAsset.hpp>

#include "AssetSystem/Assets/DirectoryAsset.hpp"

HG::Editor::AssetSystem::Assets::DirectoryAsset::DirectoryAsset(std::filesystem::path path) :
    HG::Editor::AssetSystem::Assets::AbstractAsset(std::move(path), DirectoryAsset::AssetId)
{

}
