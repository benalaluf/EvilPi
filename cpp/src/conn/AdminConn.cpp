#include "conn/AdminConn.h"
#include "utils/CommandParser.h"
#include "protocol/SocketUtil.h"
#include <thread>
#define ADMIN_INPUT_BUFFER 1024
AdminConn::AdminConn(const char *ip, int port) {
    adminSocketFD = createTCPIPv4Socket();
    serverAddress = createIPv4Address(ip, port);
}

int AdminConn::main() {
    connectToServer();

    std::thread thread(std::bind(&AdminConn::receive, this));
    thread.detach();

    return 0;
}

void AdminConn::connectToServer() {
    int result = connect(adminSocketFD, (sockaddr *) serverAddress, sizeof(struct sockaddr_in));
    localAddress = createIPv4Address(adminSocketFD);
    if (result == 0) {
        Packet packet(ADMINCONNECT,localAddress,serverAddress);
        sendPacket(adminSocketFD, packet);
        printf("CONNECTED TO SERVER\n");
    } else {
        printf("ERRRO WHILE TRYING TO CONNECT TO SEVER %d\n", result);
        exit(1);
    }
}

void AdminConn::receive() {
    while (true) {
        Packet packet;
        int status = recvPacket(adminSocketFD, &packet);;
        if (status != 0) {
            printf("error while recvPacket %d", status);
            break;
        }
        handlePacket(packet);
    }
}

void AdminConn::handlePacket(Packet packet) {
    switch (packet.getType()) {
        case (MSG):
            std::cout << "got packet: " << MsgData(packet.data, packet.getDataLength()).msg << '\n';
            break;
        case (RSH_COMMAND): {
            break;
        }
        case(SHOW):{
            MsgData msgData(packet.data, packet.getDataLength());
            std::cout << msgData.msg;
            break;
        }
        default:
            printf("got undefiend packet: %d", packet.getType());
            break;
    }
}


void AdminConn::chooseAgent(std::string ip, int port){

    agentAddress = createIPv4Address(ip.c_str(), port);
}

void AdminConn::msgAgent(std::string msg) {
    MsgData msgData(msg);
    Packet packet(MSG,localAddress,agentAddress,msgData);
    sendPacket(adminSocketFD, packet);
}

void AdminConn::showConnectedClients(){
    Packet packet(SHOW,localAddress,serverAddress);
    sendPacket(adminSocketFD, packet);
}

