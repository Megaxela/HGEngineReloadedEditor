#include <CMakeAsyncClient.hpp>

CMakeAsyncClient::CMakeAsyncClient() :
    m_connector(),
    m_cookieCounter(0),
    m_responseContainer()
{

}

CMakeConnector &CMakeAsyncClient::connector()
{
    return m_connector;
}

void CMakeAsyncClient::proceedEvents()
{
    while (true)
    {
        bool ok = false;

        const auto& message = m_connector.receiveMessage(ok);

        if (!ok)
        {
            return;
        }

        std::size_t cookie = -1;

        if (!message.cookie.empty())
        {
            cookie = std::stoll(message.cookie);
        }

        {
            auto searchResult = m_responseContainer.find(cookie);

            // Trying to proceed response callback
            if (searchResult != m_responseContainer.end())
            {
                if (message.type != Messages::Type::ErrorResponse)
                {
                    searchResult->second.responseHandler(message);
                }
                else if (searchResult->second.errorHandler)
                {
                    searchResult->second.errorHandler(message_cast<ErrorResponse>(message));
                }

                m_responseContainer.erase(searchResult);
            }
        }

        // Trying to find common callback
        {
            auto searchResult = m_commonCallbacks.find(message.type);

            if (searchResult != m_commonCallbacks.end())
            {
                searchResult->second(message);
            }
        }
    }
}

void CMakeAsyncClient::removeMessageCallback(Messages::Type type)
{
    m_commonCallbacks.erase(type);
}
