#pragma once

#include <functional>
#include <Messages/ErrorResponse.hpp>
#include <CMakeConnector.hpp>
#include <Messages/Cast.hpp>

/**
 * @brief Class, that describes async cmake
 * server client.
 */
class CMakeAsyncClient
{
public:

    using ErrorHandler = std::function<void(const ErrorResponse&)>;

    /**
     * @brief Constructor.
     */
    CMakeAsyncClient();

    /**
     * @brief Method for getting client connector.
     * @return Reference to connector object.
     */
    CMakeConnector& connector();

    /**
     * @brief Method for performing request to
     * cmake server.
     * @tparam T Callback type.
     * @param msg Message for server.
     * @param responseHandler Callback for handling message response.
     * @param errorHandler Callback for handling message error.
     */
    template<typename T>
    void performRequest(
        BasicMessage& msg,
        T responseHandler,
        ErrorHandler errorHandler=nullptr
    );

    /**
     * @brief Method for adding callback for any message
     * type. It may be useful for `hello` or `progress`
     * messages.
     * @tparam T Callback type.
     * @param type Message type.
     * @param callback Actual callback.
     */
    template<typename T>
    void setMessageCallback(
        Messages::Type type,
        T callback
    );

    /**
     * @brief Method for removing message callback
     * by type.
     * @param type Message type.
     */
    void removeMessageCallback(Messages::Type type);

    /**
     * @brief Method for checking is any message
     * available and executing callbacks if required.
     */
    void proceedEvents();

private:

    /**
     * @brief Method for performing request to
     * cmake server.
     * @tparam T Expected output type.
     * @param msg Message for server.
     * @param responseHandler Callback for handling message response.
     * @param errorHandler Callback for handling message error.
     */
    template<typename T>
    void performRequestInternal(
        BasicMessage& msg,
        std::function<void(const T&)> responseHandler,
        ErrorHandler errorHandler=nullptr
    );

    /**
     * @brief Method for adding callback for any message
     * type. It may be useful for `hello` or `progress`
     * messages.
     * @tparam T Callback type.
     * @param type Message type.
     * @param callback Actual callback.
     */
    template<typename T>
    void setMessageCallbackInternal(
        Messages::Type type,
        std::function<void(const T&)> callback
    );

    struct CallbackContainer
    {
        ErrorHandler errorHandler;
        std::function<void(const BasicMessage&)> responseHandler;
    };

    CMakeConnector m_connector;

    std::unordered_map<
        std::size_t,
        CallbackContainer
    > m_responseContainer;

    std::unordered_map<
        Messages::Type,
        std::function<void(const BasicMessage&)>
    > m_commonCallbacks;

    std::size_t m_cookieCounter;
};

template<typename T>
void CMakeAsyncClient::performRequest(BasicMessage& msg,
                                      T responseHandler,
                                      CMakeAsyncClient::ErrorHandler errorHandler)
{
    performRequestInternal(
        msg,
        std::function(responseHandler),
        std::move(errorHandler)
    );
}

template<typename T>
void CMakeAsyncClient::performRequestInternal(BasicMessage& msg,
                                              std::function<void(const T &)> responseHandler,
                                              CMakeAsyncClient::ErrorHandler errorHandler)
{
    // Changing cookie counter
    ++m_cookieCounter;

    // Adding cookie to message
    msg.cookie = std::to_string(m_cookieCounter);

    // Sending message
    m_connector.sendMessage(msg);

    // Saving response callbacks
    m_responseContainer[m_cookieCounter] = CallbackContainer{
        std::move(errorHandler),
        [responseHandler](const BasicMessage& msg)
        {
            responseHandler(message_cast<T>(msg));
        }
    };
}

template<typename T>
void CMakeAsyncClient::setMessageCallback(Messages::Type type, T callback)
{
    setMessageCallbackInternal(
        type,
        std::function(callback)
    );
}

template<typename T>
void CMakeAsyncClient::setMessageCallbackInternal(Messages::Type type, std::function<void(const T &)> callback)
{
    m_commonCallbacks[type] = [callback](const BasicMessage& msg)
    {
        callback(message_cast<T>(msg));
    };
}
