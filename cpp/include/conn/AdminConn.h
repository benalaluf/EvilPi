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

    struct sockaddr_in *agentAddress;

    void connectToServer();

    void adminInput();

    void handleAdminInput(char *buffer);

    void send();

    void receive();

    void handlePacket(Packet packet);

    void msgAgent(std::string msg);

    void chooseAgent(std::string ip, int port);

    friend class AdminTUI;
};