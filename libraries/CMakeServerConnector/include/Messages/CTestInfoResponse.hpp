#pragma once

#include <Messages/BasicMessage.hpp>

struct CTestInfoResponse : public BasicMessage
{


    static void deserialize(const nlohmann::json& json, BasicMessage& msg);
};

namespace
{
    Messages::Registrator<CTestInfoResponse> CTestInfoResponseRegistrator(
        "reply",
        "configure",
        Messages::Type::CTestInfoResponse,
        nullptr,
        &CTestInfoResponse::deserialize
    );
}

