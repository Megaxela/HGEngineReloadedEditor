#pragma once

#include <Messages/BasicMessage.hpp>
#include <vector>

struct GlobalSettingsResponse : public BasicMessage
{
    static void deserialize(const nlohmann::json& json, BasicMessage& msg);

    struct Generator
    {
        std::vector<std::string> extraGenerators;
        std::string name;
        bool platformSupport;
        bool toolsetSupport;
    };

    std::string buildDirectory;

    struct Capabilities {

        std::vector<Generator> generators;
        bool serverMode;

        struct Version {

            bool isDirty;
            uint8_t minor;
            uint8_t major;
            uint32_t patch;
            std::string string;
            std::string suffix;

        } version;

    } capabilities;

    bool checkSystemVars;
    std::string extraGenerator;
    std::string generator;
    bool debugOutput;
    std::string sourceDirectory;
    bool trace;
    bool traceExpand;
    bool warnUninitialized;
    bool warnUnused;
    bool warnUnusedCli;
};

namespace
{
    Messages::Registrator<GlobalSettingsResponse> GlobalSettingsResponseRegistrator(
        "reply",
        "globalSettings",
        Messages::Type::GlobalSettingsResponse,
        nullptr,
        &GlobalSettingsResponse::deserialize
    );
}

