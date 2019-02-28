#pragma once

#include <Messages/BasicMessage.hpp>

struct CMakeInputsRequest : public BasicMessage
{
    CMakeInputsRequest()
    {
        type = Messages::Type::CMakeInputsRequest;
    }

    static nlohmann::json serialize(const BasicMessage& msg);
};

namespace
{
    Messages::Registrator<CMakeInputsRequest> CMakeInputsRequestRegistrator(
        "cmakeInputs",
        Messages::Type::CMakeInputsRequest,
        &CMakeInputsRequest::serialize,
        nullptr
    );
}

