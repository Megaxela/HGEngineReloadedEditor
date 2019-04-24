#pragma once

// C++ STL
#include <filesystem>
#include <shared_mutex>

// Editor
#include <Tools/ThumbnailsCache.hpp>

namespace HG::Editor::AssetSystem
{
    class AssetsManager;
}

namespace HG::Editor::AssetSystem::Assets
{
    class AbstractAsset;

    using AssetPtr = std::shared_ptr<AbstractAsset>;

    /**
     * @brief Class, that describes abstract engine
     * asset.
     */
    class AbstractAsset
    {
    public:

        /**
         * @brief Asset can have one of
         * following states.
         */
        enum class State
        {
              Initial   //< Initial state.
            , NotLoaded //< Asset was found in filesystem and asset type was detected.
            , Loading   //< Asset is currently loading (thread started processing info)
            , Loaded    //< Asset was successfully loaded. But specific data was not loaded.
            , Cached    //< Asset was used recently and some specific asset data was cached.
            , Failed    //< Asset loading failed.
        };

        /**
         * @brief Constructor.
         * @param path Path.
         * @param id Asset type id.
         */
        explicit AbstractAsset(std::filesystem::path path, std::size_t id);

        /**
         * @brief Virtual destructor.
         */
        virtual ~AbstractAsset();

        /**
         * @brief Method for setting parent asset manager.
         * @param manager Pointer to asset manager.
         */
        void setAssetsManager(HG::Editor::AssetSystem::AssetsManager* manager);

        /**
         * @brief Method for getting parent asset manager.
         * @return Pointer to asset manager.
         */
        HG::Editor::AssetSystem::AssetsManager* assetsManager() const;

        /**
         * @brief Virtual method for getting asset name.
         * It will return filename without extension.
         * @return Asset name.
         */
        virtual std::string name() const;

        /**
         * @brief Method for getting asset path.
         * @return Asset path.
         */
        std::filesystem::path path() const;

        /**
         * @brief Method for getting current asset state.
         * @return State value.
         */
        State state() const;

        /**
         * @brief Method for getting asset
         * type id.
         * @return Asset type id.
         */
        std::size_t type() const;

        /**
         * @brief Method for getting asset parent.
         * @return Pointer to parent asset. Or `nullptr`
         * if there is no parent.
         */
        AbstractAsset* parent() const;

        /**
         * @brief Method for getting asset children.
         * You may modify children - only for reordering.
         */
        std::vector<AssetPtr>& children();

        /**
         * @brief Method for adding child to parent.
         */
        void addChild(AssetPtr child);

        /**
         * @brief Method, that calls `onLoad`
         * method to perform asset data loading.
         * This method can be called async.
         * If loading will be success - asset state
         * will change to `State::Loaded`. Otherwise
         * asset state will be changed to `State::Failed`.
         * This method will clean any cached data. (because
         * this method can be called at `State::Cached` state)
         * @return Loading success.
         */
        bool load();

        /**
         * @brief Method, that's used for checking is
         * loading required for asset. It returns true by default.
         */
        virtual bool isLoadingRequired() const;

        /**
         * @brief Method, that calls `onInvalidate`
         * method to perform actual assets data invalidation.
         * This method will set asset to `State::NotLoaded` state.
         * So it has to be loaded.
         */
        void invalidate();

        /**
         * @brief Method, that calls `onPostLoad`
         * method to perform any action that can be required
         * after asset loading and thumbnails cache invalidation.
         */
        void postLoad();

        /**
         * @brief Method, that returns asset icon.
         * @return Asset icon handle.
         */
        virtual HG::Editor::ThumbnailsCache::Handle icon() const;

        /**
         * @brief Method for getting last edit time.
         * If this value is lower than actual file update
         * time - then assets has old information about
         * asset.
         * @return Last update edit time of file.
         */
        std::chrono::system_clock::time_point lastEditTime() const;

    protected:

        /**
         * @brief Method for safe setting internal
         * asset state.
         * @param state New asset state.
         */
        void setState(State state);

        /**
         * @brief Method, that called on loading stage.
         * @return Loading status.
         */
        virtual bool onLoad();

        /**
         * @brief Method, that called on asset invalidation.
         * After invalidation asset has to be loaded again.
         */
        virtual void onInvalidate();

        /**
         * @brief Method, that called after all assets
         * are loaded and thumbnails cache is invalidated.
         * Do nothing by default.
         */
        virtual void onPostLoad();

        /**
         * @brief Method, that called every frame
         * if asset instance is selected (to display
         * some editing widgets at inspector window)
         */
        virtual void onInspector();

        /**
         * @brief Method, that called every frame
         * on drawing in assets window. By default
         * it just draws name. (from `name` method)
         * @return Is item opened.
         */
        virtual bool onTreeItemDraw(int flags);

    private:

        HG::Editor::AssetSystem::AssetsManager* m_assetManager;

        AbstractAsset* m_parent;
        std::vector<AssetPtr> m_children;

        State m_state;
        mutable std::shared_mutex m_stateMutex;

        std::filesystem::path m_path;
        std::size_t m_type;

        std::chrono::system_clock::time_point m_lastEditTime;
    };
}


