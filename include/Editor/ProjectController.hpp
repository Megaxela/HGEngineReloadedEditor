#pragma once

// C++ STL
#include <filesystem>

// Json
#include <nlohmann/json.hpp>

namespace HG::Editor::AssetSystem
{
    class AssetsManager;
}

namespace HG::Editor
{
    class Application;

    /**
     * @brief Class, that describes project controller.
     */
    class ProjectController
    {
    public:

        struct Metadata
        {
            std::string assetsDirectory;
            std::string activeScene;
        };

        /**
         * @brief Constructor.
         */
        explicit ProjectController(HG::Editor::Application* parent);

        /**
         * @brief Destructor.
         */
        ~ProjectController();

        /**
         * @brief Method for getting project controller application parent.
         * @return Pointer to parent application.
         */
        HG::Editor::Application* application() const;

        /**
         * @brief Method for loading project from
         * path.
         * @param path Path to project.
         */
        void load(std::filesystem::path path);

        /**
         * @brief Method for getting project controller's
         * asset manager.
         * @return Pointer to asset manager.
         */
        HG::Editor::AssetSystem::AssetsManager* assetManager();

    private:

        void parseMetadata(nlohmann::json json);

        HG::Editor::Application* m_parentApplication;

        HG::Editor::AssetSystem::AssetsManager* m_assetsManager;

        Metadata m_metadata;
    };
}


