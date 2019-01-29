// Editor
#include <Editor/ProjectMetadata.hpp>

void HG::Editor::to_json(nlohmann::json &json, const HG::Editor::ProjectMetadata &m)
{
    json = nlohmann::json{
        {"assets_directory", m.assetsDirectory},
        {"active_scene", m.activeScene},
        {"project_name", m.name}
    };
}

void HG::Editor::from_json(const nlohmann::json &json, HG::Editor::ProjectMetadata &m)
{
    json.at("assets_directory").get_to(m.assetsDirectory);
    json.at("active_scene").get_to(m.activeScene);
    json.at("project_name").get_to(m.name);
}
