#pragma once

// C++ STL
#include <memory>
#include <vector>
#include <filesystem>

namespace HG::Editor
{
    class Application;
}

namespace HG::Editor::AssetSystem::Assets
{
    class AbstractAsset;
    using AssetPtr = std::shared_ptr<AbstractAsset>;
}

namespace HG::AssetSystem
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

    private:

        std::filesystem::path m_assetsPath;

        std::vector<HG::Editor::AssetSystem::Assets::AssetPtr> m_rootAssets;

        HG::Editor::Application* m_parentApplication;
    };
}

