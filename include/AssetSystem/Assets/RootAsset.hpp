#pragma once

// Editor
#include <AssetSystem/Assets/AbstractAsset.hpp>

// Utils
#include <HG/Utils/StringTools.hpp>

namespace HG::Editor::AssetSystem::Assets
{
    /**
     * @brief Class, that describes non real asset,
     * that's just uses as assets root.
     */
    class RootAsset : public AbstractAsset
    {
    public:

        // Root asset type id (not real asset)
        static constexpr std::size_t AssetId = HG::Utils::StringTools::hash("Asset::RootAsset")

        explicit RootAsset(std::filesystem::path path);
    };
}
