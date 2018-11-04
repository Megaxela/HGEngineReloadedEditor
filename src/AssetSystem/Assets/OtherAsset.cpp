// Editor
#include <AssetSystem/Assets/OtherAsset.hpp>

HG::Editor::AssetSystem::Assets::OtherAsset::OtherAsset(std::filesystem::path path) :
    HG::Editor::AssetSystem::Assets::AbstractAsset(std::move(path), OtherAsset::AssetId)
{

}
