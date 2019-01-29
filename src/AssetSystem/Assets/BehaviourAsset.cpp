// Editor
#include <AssetSystem/Assets/BehaviourAsset.hpp>

// clang
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Frontend/FrontendActions.h>

HG::Editor::AssetSystem::Assets::BehaviourAsset::BehaviourAsset(std::filesystem::path path) :
    HG::Editor::AssetSystem::Assets::AbstractAsset(std::move(path), BehaviourAsset::AssetId)
{

}

bool HG::Editor::AssetSystem::Assets::BehaviourAsset::onLoad()
{
    
}
