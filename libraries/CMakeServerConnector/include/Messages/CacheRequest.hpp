#pragma once

#include <Messages/BasicMessage.hpp>

struct CacheRequest : public BasicMessage
{
    CacheRequest()
    {
        type = Messages::Type::CacheRequest;
    }

    static nlohmann::json serialize(const BasicMessage& msg);
};

namespace
{
    Messages::Registrator<CacheRequest> CacheRequestRegistrator(
        "cache",
        Messages::Type::CacheRequest,
        &CacheRequest::serialize,
        nullptr
    );
}

