#pragma once

#include <CMakeConnector.hpp>

/**
 * @brief Class, that describes blocking
 * client for cmake server.
 */
class CMakeBlockingClient
{
public:

    /**
     * @brief Constructor.
     */
    CMakeBlockingClient();

    /**
     * @brief Method for getting client connector.
     * @return Reference to connector object.
     */
    CMakeConnector& connector();

    /**
     * @brief Method for performing sending message
     * to server.
     * @param msg Message.
     */
    void sendMessage(const BasicMessage& msg);

    /**
     * @brief Method for receiving message from
     * server. If connection will be lost at
     * receiving - `std::runtime_error` will
     * be thrown. Message buffer is owned by internal
     * connector.
     * @return Reference to message.
     */
    const BasicMessage& receiveMessage();

private:

    CMakeConnector m_connector;
};