// Editor
#include <AssetSystem/Assets/TextAsset.hpp>

HG::Editor::AssetSystem::Assets::TextAsset::TextAsset(std::filesystem::path path) :
    HG::Editor::AssetSystem::Assets::AbstractAsset(std::move(path), TextAsset::AssetId)
{

}
