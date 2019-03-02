#pragma once

#include <Messages/BasicMessage.hpp>

struct CodeModelResponse : public BasicMessage
{
    struct IncludePath
    {
        std::string path;
    };

    struct FileGroup
    {
        std::string compileFlags;
        std::vector<std::string> defines;
        std::vector<IncludePath> includePaths;
        bool isGenerated;
        std::string language;
        std::vector<std::string> sources;
    };

    struct Target
    {
        std::vector<std::string> artifacts;
        std::string buildDirectory;

        std::vector<FileGroup> fileGroups;
        std::string fullName;
        std::string linkerLanguage;
        std::string name;
        std::string sourceDirectory;
        std::string type;
    };

    struct Project
    {
        std::string buildDirectory;
        std::string name;
        std::string sourceDirectory;

        std::vector<Target> targets;
    };

    struct Configuration
    {
        std::string name;

        std::vector<Project> projects;
    };

    std::vector<Configuration> configurations;

    static void deserialize(const nlohmann::json& json, BasicMessage& msg);
};

namespace
{
    Messages::Registrator<ComputeResponse> CodeModelResponseRegistrator(
        "reply",
        "codemodel",
        Messages::Type::CodeModelResponse,
        nullptr,
        &CodeModelResponse::deserialize
    );
}

