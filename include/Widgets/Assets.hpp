#pragma once

// C++ STL
#include <memory>
#include <functional>

// Editor
#include <Widgets/AbstractWidget.hpp>
#include <Tools/ThumbnailsCache.hpp>
#include <Tools/ImGuiWidgets.hpp>

namespace HG::Editor::AssetSystem::Assets
{
    class AbstractAsset;

    using AssetPtr = std::shared_ptr<AbstractAsset>;
}

namespace HG::Editor::Widgets::Settings
{
    struct Common;
}

namespace HG::Editor::Widgets
{
    /**
     * @brief Class, that describes assets widget.
     */
    class Assets : public AbstractWidget
    {
    public:

        /**
         * @brief Constructor.
         */
        explicit Assets(HG::Editor::Widgets::Settings::Common* settings);

    protected:

        /**
         * @brief Method, that performs
         * actual widget drawing.
         */
        void onDraw() override;

    private:

        struct SortType
        {
            std::string name;
            std::function<
                bool(
                    const HG::Editor::AssetSystem::Assets::AssetPtr& l,
                    const HG::Editor::AssetSystem::Assets::AssetPtr& r
                )
            > comparator;
        };

        /**
         * @brief Method for drawing buttons for
         * operating with assets.
         */
        void drawToolbar();

        /**
         * @brief Method for actually drawing items.
         */
        void drawItems();

        /**
         * @brief Method that performs drawing of asset and it's
         * children.
         * @param asset Pointer to asset.
         * @param isRoot Is asset root.
         */
        void drawAsset(const HG::Editor::AssetSystem::Assets::AssetPtr& asset, bool isRoot);

        /**
         * @brief Method, that performs assets sorting inside
         * asset manager.
         */
        void performSorting();

        /**
         * @brief Method, that performs sorting of asset's children.
         * It's recursive method.
         * @param asset Smart pointer to asset.
         */
        void sortAssetChildren(HG::Editor::AssetSystem::Assets::AssetPtr asset);

        /**
         * @brief Method, that performs context menu initialization.
         */
        void setupContextMenu();

        /**
         * @brief Method, that executes on Create->Folder.
         */
        void actionCreateDirectory();

        /**
         * @brief Method, that executes on Delete.
         */
        void actionDelete();

        /**
         * @brief Method, that performs fully assets reloading.
         */
        void actionReimportAll();

        /**
         * @brief Method, that executes on unimplemented methods.
         */
        void actionUnimplemented();

        HG::Editor::Widgets::Settings::Common* m_commonSettings;

        std::vector<SortType> m_sortTypes;

        std::size_t m_currentSorting;

        ImGui::Menu m_contextMenu;
    };
}