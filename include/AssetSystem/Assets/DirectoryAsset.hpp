#pragma once

// Editor
#include <AssetSystem/Assets/AbstractAsset.hpp>

// HG::Utils
#include <HG/Utils/StringTools.hpp>

namespace HG::Editor::AssetSystem::Assets
{
    /**
     * @brief Class, that describes directory in
     * assets path.
     */
    class DirectoryAsset : public HG::Editor::AssetSystem::Assets::AbstractAsset
    {
    public:

        // Directory asset type id
        static constexpr std::size_t AssetId = HG::Utils::StringTools::hash("Asset::DirectoryAsset");

        /**
         * @brief Constructor.
         * @param path Path to asset.
         */
        explicit DirectoryAsset(std::filesystem::path path);

        ThumbnailsCache::Handle icon() const override;

        bool isLoadingRequired() const override;
    };
}


