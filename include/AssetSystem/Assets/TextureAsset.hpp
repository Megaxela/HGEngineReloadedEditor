#pragma once

// Editor
#include <AssetSystem/Assets/AbstractAsset.hpp>

// HG::Utils
#include <HG/Utils/StringTools.hpp>

namespace HG::Editor::AssetSystem::Assets
{
    /**
     * @brief Class, that describes texture in
     * assets path.
     */
    class TextureAsset : public HG::Editor::AssetSystem::Assets::AbstractAsset
    {
    public:

        // Texture asset type id.
        static constexpr std::size_t AssetId = HG::Utils::StringTools::hash("Asset::TextureAsset");

        /**
         * @brief Constructor.
         * @param path Path to asset.
         */
        explicit TextureAsset(std::filesystem::path path);
    };
}


