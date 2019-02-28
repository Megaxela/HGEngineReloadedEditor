#pragma once

#include <Messages/BasicMessage.hpp>

struct SetGlobalSettingsResponse : public BasicMessage
{
    static void deserialize(const nlohmann::json& json, BasicMessage& msg);
};

namespace
{
    Messages::Registrator<SetGlobalSettingsResponse> SetGlobalSettingsResponseRegistrator(
        "reply",
        "setGlobalSettings",
        Messages::Type::SetGlobalSettingsResponse,
        nullptr,
        &SetGlobalSettingsResponse::deserialize
    );
}
