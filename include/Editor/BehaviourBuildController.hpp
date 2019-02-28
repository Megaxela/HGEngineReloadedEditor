#pragma once

// C++ STL
#include <filesystem>
#include <unordered_map>

// HG::Utils
#include <HG/Utils/filesystem_ext.hpp>

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

        struct UnitBuildInfo
        {
            // Command, that has to be executed
            // to build file
            std::string command;
            std::vector<std::string> arguments;

            // Build directory
            std::filesystem::path directory;

            // Path to source file
            std::filesystem::path file;
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
         * Also this method loads compilation database.
         * @param configurationFilePath Path to file configuration file.
         * @param fileType Type of configuration file.
         * @param buildDirectory Directory for build files.
         */
        bool configureProject(const std::filesystem::path& configurationFilePath,
                              ConfigurationFileType fileType,
                              const std::filesystem::path& buildDirectory);

        /**
         * @brief Method for getting build info for some file.
         * If there is no such build info - std::invalid_argument
         * exception will be thrown.
         * @param file Path to file.
         * @return Build info.
         */
        UnitBuildInfo getBuildInfoFor(const std::filesystem::path& file) const;

        /**
         * @brief Method for loading info from compilation databse.
         * @param compilationDatabasePath Path to compilation database.
         * @param fileType Compilation database type.
         * @return Loading success.
         */
        bool loadBuildInfoFrom(const std::filesystem::path& compilationDatabasePath, ConfigurationFileType fileType);

        /**
         * @brief Method, that performs building of source file
         * to dynamic library. If success - it will return path to
         * library. Otherwise - it will throw exception with error output.
         * @param path
         */
        std::filesystem::path buildSource(const std::filesystem::path& path);


    private:

        void parseCLICommand(const std::string& cliCommand, std::string& command, std::vector<std::string>& arguments);

        bool createLibraryLoader(const std::filesystem::path &path);

        void createCMakeLists(const std::filesystem::path& path);

        bool configureCMakeLists(const std::filesystem::path& path,
                                 const std::filesystem::path& buildDirectory);

        bool loadCMakeListsBuildInfoFrom(const std::filesystem::path& compilationDatabasePath);

        HG::Editor::Application* m_parentApplication;

        ConfigurationFileType m_loadedFileType;
        std::filesystem::path m_compilationDBPath;
        std::filesystem::path m_buildDirectory;

        std::unordered_map<
            std::filesystem::path,
            UnitBuildInfo
        > m_database;
    };
}
