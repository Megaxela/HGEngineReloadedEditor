#pragma once

// Editor
#include <AssetSystem/Assets/AbstractAsset.hpp>

// HG::Utils
#include <HG/Utils/StringTools.hpp>

namespace HG::Editor::AssetSystem::Assets
{
    /**
     * @brief Class, that describes C++ behaviour
     * asset.
     */
    class BehaviourAsset : public HG::Editor::AssetSystem::Assets::AbstractAsset
    {
    public:

        // Behaviour asset type id
        static constexpr std::size_t AssetId = HG::Utils::StringTools::hash("Asset::BehaviourAsset");

        /**
         * @brief Constructor.
         * @param path Path to asset.
         */
        explicit BehaviourAsset(std::filesystem::path path);
    };
}


