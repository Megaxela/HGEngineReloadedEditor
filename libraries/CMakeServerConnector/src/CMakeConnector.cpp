#include <CMakeConnector.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <Messages/Register.hpp>

CMakeConnector::CMakeConnector() :
    m_interface(),
    m_receiveBuffer(),
    m_firstFill(true)
{

}

Interface &CMakeConnector::interface()
{
    return m_interface;
}

void CMakeConnector::sendMessage(const BasicMessage &msg)
{
    nlohmann::json serialized;

    try
    {
        serialized = Messages::Register::instance().serializeMessage(msg);
    }
    catch (const std::runtime_error& e)
    {
        throw std::invalid_argument(e.what());
    }

    sendMessage(serialized.dump());
}

const BasicMessage &CMakeConnector::receiveMessage(bool& isReceived)
{
    // Change buffer size if required
    if (m_messageBuffer.capacity() < Messages::Register::instance().maximumMessageSize())
    {
        m_messageBuffer.reserve(Messages::Register::instance().maximumMessageSize());
    }

    auto& basicMessageReference = reinterpret_cast<BasicMessage&>(*m_messageBuffer.data());

    auto stringMessage = recvMessage();

    if (stringMessage.empty())
    {
        isReceived = false;
        return basicMessageReference;
    }

    auto jsonMessage = nlohmann::json::parse(stringMessage);

    if (!m_firstFill)
    {
        Messages::Register::instance().destructMessage(
            m_messageBuffer.data(),
            basicMessageReference.type
        );
    }

    m_firstFill = false;

    auto receivedMessageType =
        Messages::Register::instance().stringTypeToEnum(
            jsonMessage.at("type"),
            jsonMessage.value("inReplyTo", "")
        );

    Messages::Register::instance().constructMessage(
        m_messageBuffer.data(),
        receivedMessageType
    );

    Messages::Register::instance().deserializeMessage(
        jsonMessage,
        basicMessageReference
    );

    isReceived = true;

    return basicMessageReference;
}

static const std::string beginFlag = "\n[== \"CMake Server\" ==[\n";
static const std::string endFlag = "\n]== \"CMake Server\" ==]\n";

std::string CMakeConnector::recvMessage()
{
    // Reading from socket
    if (!m_interface.read(m_receiveBuffer))
    {
        return std::string();
    }

    // Searching for begin flag position
    auto beginPos = m_receiveBuffer.find(beginFlag);

    if (beginPos == std::string::npos)
    {
        return std::string();
    }

    auto endPos = m_receiveBuffer.find(endFlag);

    if (endPos == std::string::npos)
    {
        return std::string();
    }

    auto messageBegin = beginPos + beginFlag.size();

    // Getting message
    auto message = m_receiveBuffer.substr(messageBegin, endPos - messageBegin);

    // Cleaning buffer
    m_receiveBuffer.erase(beginPos, endPos + endFlag.size());

    return message;
}

void CMakeConnector::sendMessage(const std::string& message)
{
    m_interface.write(beginFlag);
    m_interface.write(message);
    m_interface.write(endFlag);
}
