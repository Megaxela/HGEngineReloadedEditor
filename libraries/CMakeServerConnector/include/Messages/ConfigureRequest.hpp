#pragma once

#include <Messages/BasicMessage.hpp>

struct ConfigureRequest : public BasicMessage
{
    ConfigureRequest()
    {
        type = Messages::Type::ConfigureRequest;
    }

    static nlohmann::json serialize(const BasicMessage& msg);

    std::vector<std::string> cacheArguments;
};

namespace
{
    Messages::Registrator<ConfigureRequest> ConfigureRequestRegistrator(
        "configure",
        Messages::Type::ConfigureRequest,
        &ConfigureRequest::serialize,
        nullptr
    );
}

