#pragma once

#include <Messages/BasicMessage.hpp>
#include <Interface.hpp>
#include <cstddef>
#include <vector>
#include <system_error>

class CMakeConnector
{
public:

    CMakeConnector();

    /**
     * @brief Method for getting connector interface.
     * @return Reference to interface.
     */
    Interface& interface();

    /**
     * @brief Method for sending message to cmake
     * server. May throw `std::invalid_argument` if it's
     * unable to serialize message.
     * @param msg Message structure.
     */
    void sendMessage(const BasicMessage& msg);

    /**
     * @brief Method for receiving message object
     * from cmake server. You have to deduce real type
     * from type basic message `type` field.
     * @param received Status, was message received from
     * connector. If result is `false` - no message returned.
     * @return Constant reference to basic message.
     * Message buffer is owned by connector class instance.
     * If you have to save message - you have to deduce type
     * after receiving and save deduced message.
     */
    const BasicMessage& receiveMessage(bool& received);

    /**
     * @brief Method, that tries to receive message
     * from cmake server. It reads [== "..." ==[ ... ]== "..." ==]
     * flags and only if full message received - returns it.
     * @return Message or empty string.
     */
    std::string recvMessage();

    /**
     * @brief Method, that appends cmake flags and
     * sends it to cmake server.
     * @param message Message.
     */
    void sendMessage(const std::string& message);
private:

    Interface m_interface;

    std::string m_receiveBuffer;

    std::vector<std::byte> m_messageBuffer;
    bool m_firstFill;
};
