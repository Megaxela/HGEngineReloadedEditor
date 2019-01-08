// Editor
#include <AssetSystem/Assets/BehaviourAsset.hpp>

HG::Editor::AssetSystem::Assets::BehaviourAsset::BehaviourAsset(std::filesystem::path path) :
    HG::Editor::AssetSystem::Assets::AbstractAsset(std::move(path), BehaviourAsset::AssetId)
{

}
