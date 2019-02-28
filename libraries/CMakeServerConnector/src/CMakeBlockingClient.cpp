#include <CMakeBlockingClient.hpp>

CMakeBlockingClient::CMakeBlockingClient() :
    m_connector()
{

}

CMakeConnector &CMakeBlockingClient::connector()
{
    return m_connector;
}

void CMakeBlockingClient::sendMessage(const BasicMessage &msg)
{
    m_connector.sendMessage(msg);
}

const BasicMessage &CMakeBlockingClient::receiveMessage()
{
    bool ok = false;

    while (true)
    {
        const BasicMessage& message = m_connector.receiveMessage(ok);

        if (!ok)
        {
            continue;
        }

        return message;
    }
}
