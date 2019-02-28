#include <Interface.hpp>

#include <iostream>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdexcept>
#include <zconf.h>


Interface::Interface() :
    m_handle(-1)
{

}

Interface::~Interface()
{
    if (isConnected())
    {
        disconnect();
    }
}

bool Interface::isConnected() const
{
    return m_handle != -1;
}

bool Interface::connect(const std::string &path)
{
    sockaddr_un name{0};

    auto socketHandle = socket (PF_LOCAL, SOCK_STREAM | SOCK_NONBLOCK, 0);

    if (socketHandle < 0)
    {
        throw std::runtime_error("Can't create socket");
    }

    name.sun_family = AF_LOCAL;

    if (sizeof(name.sun_path) < path.size())
    {
        throw std::runtime_error("Path is too big");
    }

    strncpy(name.sun_path, path.c_str(), sizeof(name.sun_path));

    name.sun_path[path.size()] = '\0';

    std::size_t size = (offsetof(sockaddr_un, sun_path)
                        + path.size());

    if (::connect(socketHandle, (sockaddr*) &name, size) < 0)
    {
        std::cout << strerror(errno) << std::endl;

        close(socketHandle);
        return false;
    }

    m_handle = socketHandle;

    return true;
}

void Interface::disconnect()
{
    close(m_handle);
    m_handle = -1;
}

void Interface::write(const std::string &s)
{
    send(m_handle, s.c_str(), s.size(), 0);
}

bool Interface::read(std::string& buffer)
{
    char temporaryBuffer[512];

    ssize_t received = recv(m_handle, temporaryBuffer, 512, 0);

    if (received == -1)
    {
        if (errno == EAGAIN)
        {
            return true;
        }

        disconnect();
        return false;
    }

    if (received)
    {
        buffer.append(temporaryBuffer, received);
    }

    return true;
}
