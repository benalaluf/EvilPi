#include "protocol/SocketUtil.h"
#include "protocol/Packet.h"
#include "protocol/PacketData.h"
#include "protocol/ProtocolDebug.h"
#include "app/Admin.h"

#include <iostream>

int main() {

//    int socketFD = createTCPIPv4Socket();
//
//    struct sockaddr_in *addr = createIPv4Address("0.0.0.0", 6969);
//    int result = connect(socketFD, (struct sockaddr *) addr, sizeof(struct sockaddr_in));
//    struct sockaddr_in *localAddr = createIPv4Address(socketFD);
//
//    Packet packet(ADMINCONNECT, localAddr, addr);
//
//    sendPacket(socketFD, packet);
//
//    char ip[1024];
//    int port;
//
//    printf("enter ip and port: ");
//    scanf("%s %d", ip, &port);
//    printf("sending to %s:%d\n", ip, port);
//    char msg[]{"hello from admin :)"};
//    MsgData msgData(msg);
//
//    Packet p1(MSG, localAddr, createIPv4Address(ip, port), msgData.serialized(), msgData.dataLength);
//
//    sendPacket(socketFD, p1);
//    printSerializedPacketMemory(p1.seriallize());
//    Packet packet1;
//    recvPacket(socketFD, &packet1);
//    printf("got packet\n");
//    printf("%s", packet1.data);

    Admin admin("0.0.0.0", 6969);
    admin.main();

    return 0;
}