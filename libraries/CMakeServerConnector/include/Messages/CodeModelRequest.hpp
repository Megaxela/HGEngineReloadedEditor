#pragma once

#include <Messages/BasicMessage.hpp>

struct CodeModelRequest : public BasicMessage
{
    CodeModelRequest()
    {
        type = Messages::Type::CodeModelRequest;
    }

    static nlohmann::json serialize(const BasicMessage& msg);
};

namespace
{
    Messages::Registrator<CodeModelRequest> CodeModelRequestRegistrator(
        "codemodel",
        Messages::Type::CodeModelRequest,
        &CodeModelRequest::serialize,
        nullptr
    );
}

