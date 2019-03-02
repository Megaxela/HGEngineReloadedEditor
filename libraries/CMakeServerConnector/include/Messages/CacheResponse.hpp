#pragma once

#include <Messages/BasicMessage.hpp>

struct CacheResponse : public BasicMessage
{
    struct CacheValue
    {
        std::string key;
        std::unordered_map<std::string, std::string> properties;
        std::string type;
        std::string value;
    };

    std::vector<CacheValue> cache;

    static void deserialize(const nlohmann::json& json, BasicMessage& msg);
};

namespace
{
    Messages::Registrator<CacheResponse> CacheResponseRegistrator(
        "reply",
        "cache",
        Messages::Type::CacheResponse,
        nullptr,
        &CacheResponse::deserialize
    );
}

