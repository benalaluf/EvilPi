#pragma once

#include "protocol/Packet.h"

class ClientConn {
public:
    ClientConn(const char *ip, int port);

    int main();

private:
    int clientSocketFD;

    struct sockaddr_in *localAddress;

    struct sockaddr_in *serverAddress;

    void connectToServer();

    void send();

    void receive();

    void handlePacket(Packet packet);

    void handleMsg(Packet packet);

};
