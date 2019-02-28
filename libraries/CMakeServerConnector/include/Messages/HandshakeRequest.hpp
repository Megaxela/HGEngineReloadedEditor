#pragma once

#include <Messages/BasicMessage.hpp>
#include <Version.hpp>
#include <Messages/Register.hpp>

struct HandshakeRequest : public BasicMessage
{
    HandshakeRequest() :
        sourceDirectory(),
        buildDirectory(),
        generator(),
        protocolVersion()
    {
        type = Messages::Type::HandshakeRequest;
    }

    static nlohmann::json serialize(const BasicMessage &msg);

    std::string sourceDirectory;
    std::string buildDirectory;
    std::string generator;
    Version protocolVersion;
};

namespace
{
    Messages::Registrator<HandshakeRequest> HandshakeRequestRegistrator(
        "handshake",
        Messages::Type::HandshakeRequest,
        &HandshakeRequest::serialize,
        nullptr
    );
}