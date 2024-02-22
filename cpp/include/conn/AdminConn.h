#pragma once

#include "protocol/Packet.h"

class AdminConn {
public:
    AdminConn(const char *ip, int port);

    int main();


private:
    int adminSocketFD;

    struct sockaddr_in *localAddress;

    struct sockaddr_in *serverAddress;

    void connectToServer();

    void adminInput();

    void handleAdminInput(char *buffer);

    void send();

    void receive();

    void handlePacket(Packet packet);
    void test();

    friend class AdminTUI;
};