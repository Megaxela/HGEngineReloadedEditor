#pragma once

#include <Messages/BasicMessage.hpp>

struct ProgressResponse : public BasicMessage
{
    std::string message;
    uint32_t minimum;
    uint32_t maximum;
    uint32_t current;
};