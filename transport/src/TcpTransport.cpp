#include "TcpTransport.hpp"

#include <cstring>
#include <stdexcept>

#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

#else

#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#endif

namespace
{
#ifdef _WIN32
    using SocketHandle = SOCKET;
    constexpr SocketHandle INVALID_SOCKET_HANDLE = INVALID_SOCKET;

    void closeSocket(SocketHandle socket)
    {
        closesocket(socket);
    }

    void ensureWinsockStarted()
    {
        static bool started = false;

        if (!started)
        {
            WSADATA wsaData {};
            int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

            if (result != 0)
            {
                throw std::runtime_error("WSAStartup failed");
            }

            started = true;
        }
    }
#else
    using SocketHandle = int;
    constexpr SocketHandle INVALID_SOCKET_HANDLE = -1;

    void closeSocket(SocketHandle socket)
    {
        close(socket);
    }
#endif
}

TcpTransport::TcpTransport(const std::string& host, int port)
    : host_(host), port_(port)
{
#ifdef _WIN32
    ensureWinsockStarted();
#endif
}

TcpTransport::~TcpTransport()
{
    disconnect();
}

bool TcpTransport::connectToTarget()
{
    if (isConnected())
    {
        return true;
    }

    struct addrinfo hints {};
    struct addrinfo* result = nullptr;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(
        host_.c_str(),
        std::to_string(port_).c_str(),
        &hints,
        &result
    );

    if (status != 0)
    {
        throw std::runtime_error("getaddrinfo failed");
    }

    for (auto* rp = result; rp != nullptr; rp = rp->ai_next)
    {
        sockfd_ = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        if (sockfd_ == INVALID_SOCKET_HANDLE)
        {
            continue;
        }

#ifdef _WIN32
        DWORD timeoutMs = 2000;

        setsockopt(
            sockfd_,
            SOL_SOCKET,
            SO_RCVTIMEO,
            reinterpret_cast<const char*>(&timeoutMs),
            sizeof(timeoutMs)
        );

        setsockopt(
            sockfd_,
            SOL_SOCKET,
            SO_SNDTIMEO,
            reinterpret_cast<const char*>(&timeoutMs),
            sizeof(timeoutMs)
        );
#else
        struct timeval timeout {};
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;

        setsockopt(sockfd_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        setsockopt(sockfd_, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
#endif

        if (connect(sockfd_, rp->ai_addr, static_cast<int>(rp->ai_addrlen)) == 0)
        {
            freeaddrinfo(result);
            return true;
        }

        disconnect();
    }

    freeaddrinfo(result);

    throw std::runtime_error(
        "Could not connect to " + host_ + ":" + std::to_string(port_)
    );
}

std::string TcpTransport::sendCommand(const std::string& command)
{
    if (!isConnected())
    {
        connectToTarget();
    }

    std::string wireCommand = command + "\n";

#ifdef _WIN32
    int sent = send(
        sockfd_,
        wireCommand.c_str(),
        static_cast<int>(wireCommand.size()),
        0
    );
#else
    ssize_t sent = send(sockfd_, wireCommand.c_str(), wireCommand.size(), 0);
#endif

    if (sent < 0)
    {
        throw std::runtime_error("send failed");
    }

    char buffer[4096] {};

#ifdef _WIN32
    int received = recv(sockfd_, buffer, sizeof(buffer) - 1, 0);
#else
    ssize_t received = recv(sockfd_, buffer, sizeof(buffer) - 1, 0);
#endif

    if (received <= 0)
    {
        throw std::runtime_error("recv failed");
    }

    buffer[received] = '\0';

    return std::string(buffer);
}

void TcpTransport::disconnect()
{
    if (sockfd_ != INVALID_SOCKET_HANDLE)
    {
        closeSocket(sockfd_);
        sockfd_ = INVALID_SOCKET_HANDLE;
    }
}

bool TcpTransport::isConnected() const
{
    return sockfd_ != INVALID_SOCKET_HANDLE;
}
