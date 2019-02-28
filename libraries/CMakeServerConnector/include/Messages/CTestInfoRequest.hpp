#pragma once

#include <Messages/BasicMessage.hpp>

struct CTestInfoRequest : public BasicMessage
{
    CTestInfoRequest()
    {
        type = Messages::Type::CTestInfoRequest;
    }

    static nlohmann::json serialize(const BasicMessage& msg);
};

namespace
{
    Messages::Registrator<CTestInfoRequest> CTestInfoRequestRegistrator(
        "ctestInfo",
        Messages::Type::CTestInfoRequest,
        &CTestInfoRequest::serialize,
        nullptr
    );
}

