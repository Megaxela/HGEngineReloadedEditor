#pragma once

// C++ STL
#include <memory>
#include <vector>
#include <filesystem>
#include <atomic>

namespace HG::Editor
{
    class Application;
}

namespace HG::Editor::AssetSystem::Assets
{
    class RootAsset;
    class AbstractAsset;
    using AssetPtr = std::shared_ptr<AbstractAsset>;
}

namespace HG::Editor::AssetSystem
{
    /**
     * @brief Class, that describes assets manager.
     */
    class AssetsManager
    {
    public:

        /**
         * @brief Constructor.
         * @param parent Pointer to parent application.
         */
        explicit AssetsManager(HG::Editor::Application* parent);

        /**
         * @brief Method for getting pointer to parent
         * application.
         * @return Pointer to parent application.
         */
        HG::Editor::Application* application() const;

        /**
         * @brief Method, that's executes every tick
         * to check fs events etc.
         */
        void proceedEvents();

        /**
         * @brief Method for setting assets path.
         * @param path Path to assets.
         */
        void setAssetsPath(std::filesystem::path path);

        /**
         * @brief Method for getting path to assets.
         * @return Path to assets.
         */
        std::filesystem::path assetsPath() const;

        /**
         * @brief Method for clearing assets from
         * manager. (Also detaches all watchers)
         */
        void clearAssets();

        /**
         * @brief Method for updating assets.
         */
        void updateAssets();

        /**
         * @brief Method for fully assets reloading and updating.
         */
        void reloadAssets();

        /**
         * @brief Method for getting manager root asset.
         * @return Pointer to abstract asset.
         */
        HG::Editor::AssetSystem::Assets::AssetPtr rootAsset() const;

    private:

        void postAssetsForLoading();

        void postAssetForLoading(HG::Editor::AssetSystem::Assets::AssetPtr asset);

        void performAssetsPostLoad(HG::Editor::AssetSystem::Assets::AssetPtr asset);

        void reloadDirectory(const std::filesystem::path& path, HG::Editor::AssetSystem::Assets::AssetPtr target);

        void removeDeletedAssets(HG::Editor::AssetSystem::Assets::AssetPtr rootAsset);

        bool m_assetsPathChanged;

        std::filesystem::path m_assetsPath;

        std::shared_ptr<HG::Editor::AssetSystem::Assets::RootAsset> m_rootAsset;

        HG::Editor::Application* m_parentApplication;

        std::atomic_int m_pendingLoadingAssets;
        bool m_isLoading;
    };
}

