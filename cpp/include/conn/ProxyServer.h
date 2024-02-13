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

    struct sockaddr_in *address;

    std::vector<ConnData> connectedClients;

    ConnData adminConn;

    bool bindServerSocket();

    void acceptConnections();

    void handleConnection(ConnData connData);

    void handleAdmin(ConnData connData);

    void handleClients();

    void handleClientPacket(Packet packet);

    void handleAdminPacket(Packet packet);

    void updateList();

    void forward(Packet packet);

    int isConnOpen(ConnData conn);
};