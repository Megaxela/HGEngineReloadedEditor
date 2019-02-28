#pragma once

#include <Messages/BasicMessage.hpp>

struct ErrorResponse : public BasicMessage
{
    std::string message;
};