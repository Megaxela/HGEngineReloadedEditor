#pragma once

#include <Messages/BasicMessage.hpp>

struct ComputeResponse : public BasicMessage
{
    static void deserialize(const nlohmann::json& json, BasicMessage& msg);
};

namespace
{
    Messages::Registrator<ComputeResponse> ComputeResponseRegistrator(
        "reply",
        "compute",
        Messages::Type::ComputeResponse,
        nullptr,
        &ComputeResponse::deserialize
    );
}

