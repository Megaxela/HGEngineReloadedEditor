#pragma once

#include <Messages/BasicMessage.hpp>

struct ConfigureResponse : public BasicMessage
{
    static void deserialize(const nlohmann::json& json, BasicMessage& msg);
};

namespace
{
    Messages::Registrator<ConfigureResponse> ConfigureResponseRegistrator(
        "reply",
        "configure",
        Messages::Type::ConfigureResponse,
        nullptr,
        &ConfigureResponse::deserialize
    );
}

