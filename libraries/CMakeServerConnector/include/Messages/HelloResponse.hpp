#pragma once

#include <Messages/BasicMessage.hpp>
#include <vector>
#include <Version.hpp>
#include <Messages/Register.hpp>
#include <Messages/Types.hpp>

struct HelloResponse : public BasicMessage
{
    static void deserialize(const nlohmann::json& json, BasicMessage& msg);

    std::vector<Version> supportedProtocolVersions;
};

namespace {
    Messages::Registrator<HelloResponse> HelloResponseRegistratir(
        "hello",
        Messages::Type::HelloResponse,
        nullptr,
        &HelloResponse::deserialize
    );
}

