#pragma once

#include <Messages/BasicMessage.hpp>

struct BaseSignalResponse : public BasicMessage
{
    std::string name;
};