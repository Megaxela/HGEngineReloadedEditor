#include <Messages/Register.hpp>
#include <Messages/BasicMessage.hpp>

Messages::Register::Register() :
    m_stringToType(),
    m_enumTypeToTypeId()
{

}

Messages::Register::~Register()
{

}

Messages::Register &Messages::Register::instance()
{
    static Messages::Register r;

    return r;
}

Messages::Type Messages::Register::stringTypeToEnum(std::string s, const std::string& inReplyTo) const
{
    s += inReplyTo;

    auto searchResult = m_stringToType.find(s);

    if (searchResult == m_stringToType.end())
    {
        return Type::Unknown;
    }

    return searchResult->second;
}

std::string Messages::Register::enumTypeToString(Messages::Type type) const
{
    auto searchResult = m_enumTypeToTypeId.find(type);

    if (searchResult == m_enumTypeToTypeId.end())
    {
        return std::string();
    }

    return searchResult->second.stringType;
}

std::size_t Messages::Register::getMessageTypeId(Messages::Type type) const
{
    auto searchResult = m_enumTypeToTypeId.find(type);

    if (searchResult == m_enumTypeToTypeId.end())
    {
        return 0;
    }

    return searchResult->second.typeId;
}

nlohmann::json Messages::Register::serializeMessage(const BasicMessage &message)
{
    auto searchResult = m_enumTypeToTypeId.find(message.type);

    if (searchResult == m_enumTypeToTypeId.end())
    {
        throw std::runtime_error("Unknown message type");
    }

    if (searchResult->second.serializeFunction == nullptr)
    {
        throw std::runtime_error("No serialization function");
    }

    return searchResult->second.serializeFunction(message);
}

void Messages::Register::deserializeMessage(const nlohmann::json &json, BasicMessage &message)
{
    std::string type = std::string(json.at("type")) + json.value("inReplyTo", "");

    auto gotEnum = stringTypeToEnum(type);

    auto searchResult = m_enumTypeToTypeId.find(gotEnum);

    if (searchResult == m_enumTypeToTypeId.end())
    {
        throw std::runtime_error("Unknown message type");
    }

    searchResult->second.deserializeFunction(json, message);
}

void Messages::Register::constructMessage(std::byte* data, Messages::Type type)
{
    auto searchResult = m_enumTypeToTypeId.find(type);

    if (searchResult == m_enumTypeToTypeId.end())
    {
        throw std::runtime_error("Unknown message type");
    }

    searchResult->second.constructFunction(data);
}

void Messages::Register::destructMessage(std::byte* data, Messages::Type type)
{
    auto searchResult = m_enumTypeToTypeId.find(type);

    if (searchResult == m_enumTypeToTypeId.end())
    {
        throw std::runtime_error("Unknown message type");
    }

    searchResult->second.destructFunction(data);
}

std::size_t Messages::Register::maximumMessageSize() const
{
    return m_biggestMessage;
}
