#pragma once

#include <Messages/BasicMessage.hpp>

struct FileSystemWatchersRequest : public BasicMessage
{
    FileSystemWatchersRequest()
    {
        type = Messages::Type::FileSystemWatchersRequest;
    }

    static nlohmann::json serialize(const BasicMessage& msg);
};

namespace
{
    Messages::Registrator<ConfigureRequest> FileSystemWatchersRequestRegistrator(
        "fileSystemWatchers",
        Messages::Type::FileSystemWatchersRequest,
        &FileSystemWatchersRequest::serialize,
        nullptr
    );
}

