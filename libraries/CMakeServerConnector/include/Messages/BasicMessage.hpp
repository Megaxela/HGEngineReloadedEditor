#pragma once

#include <string>
#include <Messages/Types.hpp>

struct BasicMessage
{
    Messages::Type type;
    std::string cookie;
    std::string replyTo;
};