// Editor
#include <AssetSystem/Assets/RootAsset.hpp>

HG::Editor::AssetSystem::Assets::RootAsset::RootAsset(std::filesystem::path path) :
    AbstractAsset(std::move(path), RootAsset::AssetId)
{

}
