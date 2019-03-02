#pragma once

#include <Messages/BasicMessage.hpp>

struct FileSystemWatchersResponse : public BasicMessage
{
    std::vector<std::string> watchedFiles;
    std::vector<std::string> watchedDirectories;

    static void deserialize(const nlohmann::json& json, BasicMessage& msg);
};

namespace
{
    Messages::Registrator<FileSystemWatchersResponse> FileSystemWatchersResponseRegistrator(
        "reply",
        "fileSystemWatchers",
        Messages::Type::FileSystemWatchersResponse,
        nullptr,
        &FileSystemWatchersResponse::deserialize
    );
}

