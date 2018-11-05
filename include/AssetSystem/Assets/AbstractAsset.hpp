#pragma once

// C++ STL
#include <filesystem>

namespace HG::Editor::AssetSystem::Assets
{
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
              LoadingPending //< Asset was found in filesystem and asset type was detected.
            , NotLoaded      //< After trying to load asset - some error was received.
            , Loaded         //< Asset loads some information for displaying. Or/And prepared to
                             //< be dragged onto scene.
            , Cached         //< Asset was used recently and some specific asset data
                             //< can be cached
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
        std::vector<AbstractAsset*>& children();

        /**
         * @brief Method for setting parent for asset.
         * @param asset Pointer to new parent.
         */
        void setParent(AbstractAsset* parent);

        /**
         * @brief Method, that calls `onLoad`
         * method to perform asset data loading.
         * This method can be called async.
         * If loading will be success - asset state
         * will change to `State::Loaded`. Otherwise
         * asset state will be changed to `State::NotLoaded`.
         * This method will clean any cached data. (because
         * this method can be called at `State::Cached` state)
         * @return Loading success.
         */
        bool load() const;

    protected:

        /**
         * @brief Method, that called on loading stage.
         * @return Loading status.
         */
        virtual bool onLoad();

        /**
         * @brief Method, that called every frame
         * if asset instance is selected (to display
         * some editing widgets at inspector window)
         */
        virtual void onInspector();

    private:

        AbstractAsset* m_parent;
        std::vector<AbstractAsset*> m_children;

        State m_state;
        std::filesystem::path m_path;
        std::size_t m_type;
    };
}


