#pragma once

#include "conn/ProxyServer.h"
#include "protocol/Packet.h"
#include "protocol/Networking.h"
#include "conn/ConnData.h"
#include <vector>


class ProxyServer {
public:

    ProxyServer(const char *ip, int port);

    int main();


private:

    int serverSocketFD;

    std::vector<ConnData> connections;

    struct sockaddr_in *address;

    bool bindServerSocket();

    void acceptConnections();

    void handleConnection(ConnData connData);

    void handleAdmin(ConnData connData);

    void handleAgent(ConnData connData);

    void handleAgentPacket(Packet packet);

    void handleAdminPacket(Packet packet);

    void forward(Packet packet);
};