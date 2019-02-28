#include <iostream>
#include <Interface.hpp>
#include <CMakeConnector.hpp>
#include <thread>
#include <Messages/HandshakeRequest.hpp>
#include <Messages/HandshakeResponse.hpp>
#include <Messages/ErrorResponse.hpp>
#include <Messages/Cast.hpp>
#include <Messages/HelloResponse.hpp>
#include <CMakeBlockingClient.hpp>
#include <CMakeAsyncClient.hpp>
#include <Messages/GlobalSettingsRequest.hpp>
#include <Messages/GlobalSettingsResponse.hpp>

int blockingExample()
{
    CMakeBlockingClient client;

    if (!client.connector().interface().connect("/tmp/cmake-pipe"))
    {
        std::cout << "Can't connect to cmake pipe" << std::endl;
        return 1;
    }

    {
        const BasicMessage& message = client.receiveMessage();

        if (message.type == Messages::Type::HelloResponse)
        {
            std::cout << "Hello received" << std::endl;

            const auto& helloResponse = message_cast<HelloResponse>(message);

            std::cout << "Supported version: " << std::endl;
            for (auto& version : helloResponse.supportedProtocolVersions)
            {
                std::cout << version.major << ", " << version.minor << std::endl;
            }
        }
        else
        {
            std::cout << "Received message with unknown type" << std::endl;
        }
    }

    std::cout << "Sending handshake" << std::endl;

    HandshakeRequest handshakeRequest;
    handshakeRequest.generator = "Unix Makefiles";
    handshakeRequest.buildDirectory = "/home/ushanovalex/Development/Projects/HGEngine/Sample New/Build";
    handshakeRequest.sourceDirectory = "/home/ushanovalex/Development/Projects/HGEngine/Sample New";
    handshakeRequest.protocolVersion = Version{2, 1};

    client.sendMessage(handshakeRequest);

    {
        const BasicMessage &message = client.receiveMessage();

        if (message.type == Messages::Type::HandshakeResponse)
        {
            std::cout << "Handshake response received" << std::endl;
        }
    }

    return 0;
}

int asyncExample()
{
    CMakeAsyncClient client;

    client.setMessageCallback(
        Messages::Type::HelloResponse,
        [](const HelloResponse& helloResponse)
        {
            std::cout << "Hello received" << std::endl;

            std::cout << "Supported protocol versions: " << std::endl;

            for (auto& version : helloResponse.supportedProtocolVersions)
            {
                std::cout << "    (" << version.major << '.' << version.minor << ")" << std::endl;
            }
        }
    );

    if (!client.connector().interface().connect("/tmp/cmake-pipe"))
    {
        std::cout << "Can't connect to cmake pipe" << std::endl;
        return 1;
    }

    std::cout << "Sending handshake" << std::endl;

    HandshakeRequest handshakeRequest;
    handshakeRequest.generator = "Unix Makefiles";
    handshakeRequest.buildDirectory = "/home/ushanovalex/Development/Projects/HGEngine/Sample New/Build";
    handshakeRequest.sourceDirectory = "/home/ushanovalex/Development/Projects/HGEngine/Sample New";
    handshakeRequest.protocolVersion = Version{2, 1};

    bool running = true;

    auto errorCallback =
        [](const ErrorResponse& error)
        {
            std::cout << "Error received: " << error.message << std::endl;
        };

    client.performRequest(
        handshakeRequest,
        [&running, &client, errorCallback](const HandshakeResponse& handshake) -> void
        {
            std::cout << "Handshake response received" << std::endl;

            GlobalSettingsRequest request;

            client.performRequest(
                request,
                [&running](const GlobalSettingsResponse& response)
                {
                    std::cout << "Global settings response received:" << std::endl;

                    std::cout << "Source dir: " << response.sourceDirectory << std::endl;

                    running = false;
                },
                errorCallback
            );
        },
        errorCallback
    );

    while (running)
    {
        client.proceedEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

int main()
{
    return asyncExample();
}