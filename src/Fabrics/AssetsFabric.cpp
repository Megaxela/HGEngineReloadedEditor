// Editor
#include <Fabrics/AssetsFabric.hpp>
#include <AssetSystem/Assets/OtherAsset.hpp>
#include <AssetSystem/Assets/TextAsset.hpp>
#include <AssetSystem/Assets/DirectoryAsset.hpp>

// ALogger
#include <CurrentLogger.hpp>
#include <AssetSystem/Assets/TextureAsset.hpp>

HG::Editor::Fabrics::AssetsFabric::AssetsFabric() :
    m_data()
{

}

HG::Editor::Fabrics::AssetsFabric::AssetPtr HG::Editor::Fabrics::AssetsFabric::create(std::filesystem::path path)
{
    std::error_code error;

    auto status = std::filesystem::status(path, error);

    if (error)
    {
        Error() << "Can't get \"" << path << "\" asset status. Error: " << error.message();
        return nullptr;
    }

    if (status.type() == std::filesystem::file_type::directory)
    {
        return std::make_shared<HG::Editor::AssetSystem::Assets::DirectoryAsset>(std::move(path));
    }

    auto extension = path.extension().string();

    if (extension.empty())
    {
        return nullptr;
    }

    for (const auto& [exts, functor] : m_data)
    {
        auto iter = exts.find(extension);

        if (iter == exts.end())
        {
            continue;
        }

        return functor(std::move(path));
    }

    return std::make_shared<HG::Editor::AssetSystem::Assets::OtherAsset>(std::move(path));
}

void HG::Editor::Fabrics::AssetsFabric::clear()
{
    m_data.clear();
}

void HG::Editor::Fabrics::AssetsFabric::registrateDefault()
{
    registrate<HG::Editor::AssetSystem::Assets::TextAsset>({".txt"});
    registrate<HG::Editor::AssetSystem::Assets::TextureAsset>({".png", ".tga", ".bmp"});
}
