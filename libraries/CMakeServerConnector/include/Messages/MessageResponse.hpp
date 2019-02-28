#pragma once

#include <Messages/BasicMessage.hpp>

struct MessageResponse : public BasicMessage
{
    std::string message;
    std::string title;
};