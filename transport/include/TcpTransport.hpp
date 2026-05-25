#pragma once

#include "ITransport.hpp"

#include <string>

class TcpTransport : public ITransport {
public:
    TcpTransport(const std::string& host, int port);
    ~TcpTransport() override;

    bool connectToTarget() override;
    std::string sendCommand(const std::string& command) override;

    void disconnect();
    bool isConnected() const;

private:
    std::string host_;
    int port_;

#ifdef _WIN32
    using SocketHandle = unsigned long long;
    SocketHandle sockfd_ = ~0ULL;
#else
    int sockfd_ = -1;
#endif
};