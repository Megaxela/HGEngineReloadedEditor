#pragma once

// Editor
#include <Editor/ProjectMetadata.hpp>

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
    class BehaviourBuildController;
    class Application;

    /**
     * @brief Class, that describes project controller.
     */
    class ProjectController
    {
    public:

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
         * Can throw `std::invalid_argument` exception
         * if project can't be opened by provided path.
         * @param path Path to project.
         */
        void load(std::filesystem::path path);

        /**
         * @brief Method for saving project in desired
         * directory.
         * Can throw `std::runtime_error` if saving can't be
         * performed.
         */
        void save();

        /**
         * Method for creating project in provided path.
         * Can throw `std::invalid_argument` exception
         * if project can't be created by provided path.
         * @param path Path to existing or non existing
         * directory for new project.
         * @param name Project name.
         */
        void create(std::filesystem::path path, std::string name);

        /**
         * @brief Method for closing currently opened project
         * without saving.
         */
        void close();

        /**
         * @brief Method for getting path to project root directory.
         */
        std::filesystem::path projectPath() const;

        /**
         * @brief Method for getting project controller's
         * asset manager.
         * @return Pointer to asset manager.
         */
        HG::Editor::AssetSystem::AssetsManager* assetManager();

        /**
         * @brief Method for getting pointer to project's behaviour
         * build controller.
         * @return Pointer to behaviour build controller.
         */
        HG::Editor::BehaviourBuildController* behaviourBuildController();

        /**
         * @brief Method for getting pointer to project's metadata.
         */
        HG::Editor::ProjectMetadata* metadata();

        /**
         * @brief Method for checking is project opened currently.
         */
        bool isOpened() const;

    private:

        std::filesystem::path m_projectPath;

        HG::Editor::Application* m_parentApplication;

        HG::Editor::AssetSystem::AssetsManager* m_assetsManager;

        HG::Editor::BehaviourBuildController* m_behaviourBuildController;

        ProjectMetadata m_metadata;
    };
}


