#pragma once

#include <Messages/BasicMessage.hpp>

struct SetGlobalSettingsRequest : public BasicMessage
{
    SetGlobalSettingsRequest()
    {
        type = Messages::Type::SetGlobalSettingsRequest;
    }

    static nlohmann::json serialize(const BasicMessage& msg);

    bool checkSystemVars;
    bool debugOutput;
    bool trace;
    bool traceExpand;
    bool warnUninitialized;
    bool warnUnused;
    bool warnUnusedCli;
};

namespace
{
    Messages::Registrator<SetGlobalSettingsRequest> SetGlobalSettingsRequestRegistrator(
        "setGlobalSettings",
        Messages::Type::SetGlobalSettingsRequest,
        &SetGlobalSettingsRequest::serialize,
        nullptr
    );
}
