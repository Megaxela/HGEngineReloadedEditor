#pragma once

// C++ STL
#include <string>

// json
#include <nlohmann/json.hpp>

namespace HG::Editor
{
    struct ProjectMetadata
    {
        std::string name;
        std::string assetsDirectory;
        std::string activeScene;
    };

    // Serializing
    void to_json(nlohmann::json& json, const ProjectMetadata& m);
    void from_json(const nlohmann::json& json, ProjectMetadata& m);
}