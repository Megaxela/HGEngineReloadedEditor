
// C++ STL
#include <fstream>

// Editor
#include <Editor/EngineInfo.hpp>

// nlohmann
#include <nlohmann/json.hpp>

HG::Editor::EngineInfo::EngineInfo() :
    m_modules()
{

}

void HG::Editor::EngineInfo::load(const std::filesystem::path &path)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        throw std::invalid_argument("Can't open engine info file at \"" + path.string() + "\"");
    }

    auto parsed = nlohmann::json::parse(file);

    auto modules = parsed["modules"];

    for (auto&& moduleObject : modules)
    {
        ModuleInfo module;

        moduleObject.at("name").get_to(module.name);
        moduleObject.at("library").get_to(module.libraryName);
        moduleObject.at("dependencies").get_to(module.dependencies);
    }
}

void HG::Editor::EngineInfo::getAvailableModules(std::vector<std::string> &data) const
{
    for (auto&& [key, value] : m_modules)
    {
        data.emplace_back(key);
    }
}

std::vector<std::string> HG::Editor::EngineInfo::getAvailableModules() const
{
    std::vector<std::string> result;

    getAvailableModules(result);

    return result;
}

HG::Editor::EngineInfo::ModuleInfo HG::Editor::EngineInfo::getModuleInfo(const std::string &moduleName) const
{
    auto findResult = m_modules.find(moduleName);

    if (findResult == m_modules.end())
    {
        throw std::invalid_argument("There is no module \"" + moduleName + "\"");
    }

    return findResult->second;
}
