#pragma once

// Editor
#include <AssetSystem/Assets/AbstractAsset.hpp>

// HG::Utils
#include <HG/Utils/StringTools.hpp>

namespace HG::Editor::AssetSystem::Assets
{
    /**
     * @brief Class, that describes any asset,
     * that can be used as text.
     */
    class TextAsset : public HG::Editor::AssetSystem::Assets::AbstractAsset
    {
    public:

        // Text asset type id
        static constexpr std::size_t AssetId = HG::Utils::StringTools::hash("Asset::TextAsset");

        /**
         * @brief Constructor.
         * @param path Path to asset.
         */
        explicit TextAsset(std::filesystem::path path);
    };
}

