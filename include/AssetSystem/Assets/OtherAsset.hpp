#pragma once

// Editor
#include <AssetSystem/Assets/AbstractAsset.hpp>

// HG::Utils
#include <HG/Utils/StringTools.hpp>

namespace HG::Editor::AssetSystem::Assets
{
    /**
     * @brief Class, that describes any asset, that can't be delegated
     * to known assets.
     */
    class OtherAsset : public HG::Editor::AssetSystem::Assets::AbstractAsset
    {
    public:

        // Other asset type id
        static constexpr std::size_t AssetId = HG::Utils::StringTools::hash("Asset::OtherAsset");

        /**
         * @brief Constructor.
         * @param path Path to asset.
         */
        explicit OtherAsset(std::filesystem::path path);
    };
}
