#pragma once

// C++ STL
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

namespace HG::Editor
{
    /**
     * @brief Class, that provides information
     * about builtin HGEngine. Information
     * is taken from `engine_meta.json` file.
     */
    class EngineInfo
    {
    public:

        struct ModuleInfo
        {
            std::string name;
            std::string libraryName;
            std::vector<std::string> dependencies;
        };

        /**
         * @brief Constructor.
         */
        EngineInfo();

        /**
         * @brief Method for loading engine info.
         * If there is no such file `std::invalid_argument` exception
         * will be thrown.
         */
        void load(const std::filesystem::path& path);

        /**
         * @brief Method for getting available modules to
         * vector.
         * @param data Reference to vector with results.
         */
        void getAvailableModules(std::vector<std::string>& data) const;

        /**
         * @brief Method for getting available modules.
         * @return Vector with available modules names.
         */
        std::vector<std::string> getAvailableModules() const;

        /**
         * @brief Method for getting module info by module name.
         * If there is no such module `std::invalid_argument` exception
         * will be thrown.
         * @param moduleName Request module name.
         * @return Module info.
         */
        ModuleInfo getModuleInfo(const std::string& moduleName) const;

    private:

        std::unordered_map<
            std::string,
            ModuleInfo
        > m_modules;
    };
}
