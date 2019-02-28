#pragma once

#include <Messages/BaseSignalResponse.hpp>
#include <vector>

struct FileChangeSignalResponse : public BaseSignalResponse
{
    std::string path;
    std::vector<std::string> properties;
};