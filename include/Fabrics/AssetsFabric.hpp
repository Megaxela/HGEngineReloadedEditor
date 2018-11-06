#pragma once

// C++ STL
#include <unordered_set>
#include <memory>
#include <filesystem>
#include <functional>
#include <map>
#include <set>

namespace HG::Editor::AssetSystem::Assets
{
    class AbstractAsset;
}

namespace HG::Editor::Fabrics
{
    /**
     * @brief Class that describes fabric for
     * project assets.
     */
    class AssetsFabric
    {
    public:

        using AssetPtr = std::shared_ptr<HG::Editor::AssetSystem::Assets::AbstractAsset>;

        /**
         * @brief Constructor.
         */
        AssetsFabric();

        /**
         * @brief Method for creating asset from path to asset.
         * If path points to directory - `Assets::DirectoryAsset` will be created.
         * If asset type can't be detected - `Assets::OtherAsset` will be
         * created. If there is some error - `nullptr` will be returned.
         * @param path Path to asset.
         * @return Shared pointer to asset.
         */
        AssetPtr create(std::filesystem::path path);

        /**
         * @brief Method, that performs registration of some
         * asset into this fabric.
         * @tparam Asset Asset type.
         * @param extensions Extension for identifying asset.
         */
        template<typename AssetType>
        void registrate(const std::set<std::string>& extensions)
        {
            m_data[extensions] = [](std::filesystem::path path) -> AssetPtr
            {
                return std::make_shared<AssetType>(std::move(path));
            };
        }

        /**
         * @brief Method, that performs clearing of
         * fabric data.
         */
        void clear();

        /**
         * @brief Method, that performs registration of
         * default assets.
         */
        void registrateDefault();

    private:

        std::map<
            std::set<std::string>,
            std::function<AssetPtr(std::filesystem::path)>
        > m_data;
    };
}


