#pragma once

#include "protocol/Packet.h"

class AgentConn {
public:
    AgentConn(const char *ip, int port);

    int main();

private:
    int agentSocketFD;

    struct sockaddr_in *localAddress;

    struct sockaddr_in *serverAddress;

    void connectToServer();

    void send();

    void receive();

    void handlePacket(Packet packet);

    void handleMsg(Packet packet);

};
