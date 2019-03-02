#pragma once

#include <Messages/BasicMessage.hpp>

struct CMakeInputsResponse : public BasicMessage
{
    struct BuildFile
    {
        bool isCMake;
        bool isTemporary;
        std::vector<std::string> sources;
    };

    std::vector<BuildFile> buildFiles;
    std::string cmakeRootDirectory;
    std::string sourceDirectory;

    static void deserialize(const nlohmann::json& json, BasicMessage& msg);
};

namespace
{
    Messages::Registrator<CMakeInputsResponse> CMakeInputsResponseRegistrator(
        "reply",
        "cmakeInputs",
        Messages::Type::CMakeInputsResponse,
        nullptr,
        &CMakeInputsResponse::deserialize
    );
}

