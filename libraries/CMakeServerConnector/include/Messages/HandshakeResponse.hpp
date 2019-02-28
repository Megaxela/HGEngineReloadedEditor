#pragma once

#include <Messages/BasicMessage.hpp>
#include <Version.hpp>
#include <Messages/Register.hpp>

struct HandshakeResponse : public BasicMessage
{
    static void deserialize(const nlohmann::json& json, BasicMessage& msg);
};

namespace
{
    Messages::Registrator<HandshakeResponse> HandshakeResponseRegistrator(
        "reply",
        "handshake",
        Messages::Type::HandshakeResponse,
        nullptr,
        &HandshakeResponse::deserialize
    );
}