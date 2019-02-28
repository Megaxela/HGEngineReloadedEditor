#pragma once

#include <Messages/BasicMessage.hpp>
#include <Messages/Register.hpp>

template<typename CastToType, typename CastFromType>
std::conditional_t<
    std::is_const<
        CastFromType
    >::value,
    std::add_const_t<
        CastToType
    >,
    CastToType
>& message_cast(CastFromType& message)
{
    static_assert(
        std::is_base_of<
            CastFromType,
            CastToType
        >::value,
        "message_cast only can perform upcast for messages"
    );

    std::size_t realMessageTypeId = Messages::Register::instance().getMessageTypeId(message.type);

    if (typeid(CastToType).hash_code() != realMessageTypeId)
    {
        throw std::bad_cast();
    }

    return static_cast<std::conditional_t<
        std::is_const<
            CastFromType
        >::value,
        std::add_const_t<
            CastToType
        >,
        CastToType
    >&>(message);
}