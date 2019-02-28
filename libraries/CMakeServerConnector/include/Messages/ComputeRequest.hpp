#pragma once

#include <Messages/BasicMessage.hpp>

struct ComputeRequest : public BasicMessage
{
    ComputeRequest()
    {
        type = Messages::Type::ComputeRequest;
    }

    static nlohmann::json serialize(const BasicMessage& msg);
};

namespace
{
    Messages::Registrator<ComputeRequest> ComputeRequestRegistrator(
        "compute",
        Messages::Type::ComputeRequest,
        &ComputeRequest::serialize,
        nullptr
    );
}

