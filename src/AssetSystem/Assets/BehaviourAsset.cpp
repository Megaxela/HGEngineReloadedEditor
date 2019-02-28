// Editor
#include <AssetSystem/Assets/BehaviourAsset.hpp>
#include <Editor/Application.hpp>
#include <AssetSystem/AssetsManager.hpp>
#include <Editor/ProjectController.hpp>
#include <Editor/BehaviourBuildController.hpp>
#include <CodeAnalyze/BehaviourSearchVisitor.hpp>

// clang
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Frontend/FrontendActions.h>
#include <exec-stream.h>

// ALogger
#undef Error
#include <CurrentLogger.hpp>

HG::Editor::AssetSystem::Assets::BehaviourAsset::BehaviourAsset(std::filesystem::path path) :
    HG::Editor::AssetSystem::Assets::AbstractAsset(std::move(path), BehaviourAsset::AssetId)
{

}

bool HG::Editor::AssetSystem::Assets::BehaviourAsset::onLoad()
{
    // todo: Add configurator for external setup
    // todo: Remove std::filesystem::current_path() from database
    // Initializing clang backend
    clang::tooling::FixedCompilationDatabase database(
        ".",
        {
            "-Xclang",
            "-std=c++17",
            "-I" + std::filesystem::current_path().string() + "/engine/include/"
        }
    );

    // Creating sources
    std::vector<std::string> files = {path()};

    // Creating tool
    clang::tooling::ClangTool tool(database, files);

    CodeAnalyze::BehavioursSearchVisitor::Result result;

    // Creating frontend action
    auto actionFabric = CodeAnalyze::BehavioursSearchVisitor::createFabric(&result);

    // Running frontend
    auto executionResult = tool.run(actionFabric.get());

    if (executionResult == 1 ||
        result.names.empty())
    {
        return false;
    }

    if (result.names.size() == 1)
    {
        // todo: Add behaviour asset registration here
        setName(result.names[0]);
        return true;
    }

    children().clear();
    for (auto& el : result.names)
    {
        auto asset = std::make_shared<BehaviourAsset>(path());

        asset->setName(el);

        // todo: Add behaviour asset registration here
        addChild(asset);
    }

    return false;
}

void HG::Editor::AssetSystem::Assets::BehaviourAsset::setName(std::string name)
{
    m_name = std::move(name);
}

std::string HG::Editor::AssetSystem::Assets::BehaviourAsset::name() const
{
    if (m_name.empty())
    {
        return AbstractAsset::name();
    }

    return m_name;
}
