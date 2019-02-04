#pragma once

// C++ STL
#include <filesystem>

namespace HG::Editor
{
    class Application;

    /**
     * @brief Class, that describes controller
     * for behaviour configuring and building.
     * It contains compilation database and
     * responsible for compilation database update if
     * required.
     */
    class BehaviourBuildController
    {
    public:

        enum class ConfigurationFileType
        {
            CMakeLists
        };

        /**
         * @brief Constructor.
         */
        explicit BehaviourBuildController(HG::Editor::Application* parent);

        /**
         * @brief Method, that performs creation of configuration
         * file of desired type.
         * May throw `std::invalid_argument` exception if file
         * can't be created.
         * @param path Path to configuration file.
         * @param fileType Configuration file type.
         */
        void createConfigurationFile(const std::filesystem::path& path, ConfigurationFileType fileType);

        /**
         * @brief Method, that performs project configuration.
         * @param configurationFilePath Path to file configuration file.
         * @param fileType Type of configuration file.
         * @param buildDirectory Directory for build files.
         */
        bool configureProject(const std::filesystem::path& configurationFilePath,
                              ConfigurationFileType fileType,
                              const std::filesystem::path& buildDirectory);

    private:

        void createCMakeLists(const std::filesystem::path& path);

        bool configureCMakeLists(const std::filesystem::path& path,
                                 const std::filesystem::path& buildDirectory);

        HG::Editor::Application* m_parentApplication;
    };
}
