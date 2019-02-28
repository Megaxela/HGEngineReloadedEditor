#pragma once

namespace Messages
{
    enum class Type
    {
        Unknown,
        ErrorResponse,
        HelloResponse,
        HandshakeRequest,
        HandshakeResponse,
        GlobalSettingsRequest,
        GlobalSettingsResponse,
        SetGlobalSettingsRequest,
        SetGlobalSettingsResponse,
        ConfigureRequest,
        ConfigureResponse,
        ComputeRequest,
        ComputeResponse,
        CodeModelRequest,
        CodeModelResponse,
        CTestInfoRequest,
        CTestInfoResponse,
        CMakeInputsRequest,
        CMakeInputsResponse,
        CacheRequest,
        CacheResponse,
        FileSystemWatchersRequest,
        FileSystemWatchersResponse
    };
}


