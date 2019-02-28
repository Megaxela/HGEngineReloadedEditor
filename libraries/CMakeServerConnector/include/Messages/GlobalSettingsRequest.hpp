#pragma once

#include <Messages/BasicMessage.hpp>

struct GlobalSettingsRequest : public BasicMessage
{
    GlobalSettingsRequest()
    {
        type = Messages::Type::GlobalSettingsRequest;
    }

    static nlohmann::json serialize(const BasicMessage& msg);
};

namespace
{
    Messages::Registrator<GlobalSettingsRequest> GlobalSettingsRequestRegistrator(
        "globalSettings",
        Messages::Type::GlobalSettingsRequest,
        &GlobalSettingsRequest::serialize,
        nullptr
    );
}

