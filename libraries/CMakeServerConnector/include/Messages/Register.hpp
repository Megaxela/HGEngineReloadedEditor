#pragma once

#include <nlohmann/json.hpp>
#include <unordered_map>
#include <Messages/Types.hpp>

class BasicMessage;

namespace Messages
{
    /**
     * @brief Class, that describes messages fabric.
     * It's singleton.
     */
    class Register
    {
    public:

        using SerializeFunction = std::function<nlohmann::json(const BasicMessage&)>;
        using DeserializeFunction = std::function<void(const nlohmann::json&, BasicMessage&)>;

        /**
         * @brief Constructor.
         */
        Register();

        /**
         * @brief Destructor.
         */
        ~Register();

        /**
         * @brief Method for getting register
         * singleton value.
         * @return
         */
        static Register& instance();

        /**
         * @brief Method for getting enum message type
         * from string message type.
         * @param s String type value.
         * @return Type num value. If unknown string type received - Type::Unknown
         * will be returned.
         */
        Type stringTypeToEnum(std::string type, const std::string& inReplyTo=std::string()) const;

        /**
         * @brief Method for getting string type value.
         * @param type Enum type value.
         * @return String type value. Empty string is returned if
         * unknown type received.
         */
        std::string enumTypeToString(Type type) const;

        /**
         * @brief Method for getting type id of message type's type.
         * @param type Type enum value.
         * @return Type id of message type.
         */
        std::size_t getMessageTypeId(Type type) const;

        /**
         * @brief Method for message serialization.
         * If message type is unknown - std::invalid_argument exception
         * will be thrown.
         * @param message Reference to message.
         * @return Json.
         */
        nlohmann::json serializeMessage(const BasicMessage& message);

        /**
         * @brief Method for message deserialization.
         * @param json Json.
         * @param message Message.
         */
        void deserializeMessage(const nlohmann::json& json, BasicMessage& message);

        /**
         * @brief Method for constructing message in buffer.
         * @param data Pointer to data. Data must be >= to `maximumMessageSize` result.
         * @param type Message type.
         */
        void constructMessage(std::byte* data, Type type);

        /**
         * @brief Method for destructing message in buffer.
         * @param data Pointer to data.
         * @param type Message type.
         */
        void destructMessage(std::byte* data, Type type);

        /**
         * @brief Method for adding message record to register.
         * @tparam T Actual message type.
         * @param stringType Message string type.
         * @param enumType
         * @param serializeFunction
         */
        template<typename T>
        void add(std::string stringType,
                 std::string inReplyTo,
                 Type enumType,
                 SerializeFunction serializeFunction,
                 DeserializeFunction deserializeFunction);

        /**
         * @brief Method for getting size of biggest message
         * structure in bytes.
         * @return Size in bytes.
         */
        std::size_t maximumMessageSize() const;

    private:

        using ConstructFunction = std::function<void(std::byte*)>;
        using DestructFunction = std::function<void(std::byte*)>;

        struct MessageTypeData
        {
            std::string stringType;

            std::size_t typeId;
            SerializeFunction serializeFunction;
            DeserializeFunction deserializeFunction;

            ConstructFunction constructFunction;
            DestructFunction destructFunction;
        };

        std::unordered_map<
            std::string,
            Type
        > m_stringToType;

        std::unordered_map<Type, MessageTypeData> m_enumTypeToTypeId;
        std::size_t m_biggestMessage;
    };

    template<typename T>
    void Register::add(std::string stringType,
                       std::string inReplyTo,
                       Type enumType,
                       Register::SerializeFunction serializeFunction,
                       Register::DeserializeFunction deserializeFunction)
    {
        auto typeCopy = stringType;
        typeCopy += inReplyTo;

        m_stringToType.emplace(std::move(typeCopy), enumType);
        m_enumTypeToTypeId.emplace(
            enumType,
            MessageTypeData{
                std::move(stringType),
                typeid(T).hash_code(),
                std::move(serializeFunction),
                std::move(deserializeFunction),
                [](std::byte* data) -> void
                { new (data) T; },
                [](std::byte* data) -> void
                { (*reinterpret_cast<T*>(data)).~T(); }
            }
        );

        m_biggestMessage = std::max(m_biggestMessage, sizeof(T));
    }

    template<typename T>
    class Registrator
    {
    public:
        Registrator(std::string stringType,
                    Type enumType,
                    Register::SerializeFunction serializeFunction,
                    Register::DeserializeFunction deserializeFunction);

        Registrator(std::string stringType,
                    std::string inReplyTo,
                    Type enumType,
                    Register::SerializeFunction serializeFunction,
                    Register::DeserializeFunction deserializeFunction);
    };

    template<typename T>
    Registrator<T>::Registrator(std::string stringType,
                                Type enumType,
                                Register::SerializeFunction serializeFunction,
                                Register::DeserializeFunction deserializeFunction)
    {
        Messages::Register::instance().add<T>(
            std::move(stringType),
            std::string(),
            enumType,
            std::move(serializeFunction),
            std::move(deserializeFunction)
        );
    }

    template<typename T>
    Registrator<T>::Registrator(std::string stringType,
                                std::string inReplyTo,
                                Type enumType,
                                Register::SerializeFunction serializeFunction,
                                Register::DeserializeFunction deserializeFunction)
    {
        Messages::Register::instance().add<T>(
            std::move(stringType),
            std::move(inReplyTo),
            enumType,
            std::move(serializeFunction),
            std::move(deserializeFunction)
        );
    }

}