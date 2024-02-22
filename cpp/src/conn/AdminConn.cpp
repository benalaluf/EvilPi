#include "conn/AdminConn.h"
#include "utils/CommandParser.h"
#include "protocol/SocketUtil.h"
#define ADMIN_INPUT_BUFFER 1024
AdminConn::AdminConn(const char *ip, int port) {
    adminSocketFD = createTCPIPv4Socket();
    serverAddress = createIPv4Address(ip, port);
    localAddress = createIPv4Address(adminSocketFD);
}

int AdminConn::main() {
    connectToServer();
    return 0;
}

void AdminConn::connectToServer() {
    int result = connect(adminSocketFD, (sockaddr *) serverAddress, sizeof(struct sockaddr_in));
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

}

void AdminConn::send() {

}

void AdminConn::handlePacket(Packet packet) {

}
void AdminConn::test() {
    std::cout << "test\n";
}

void AdminConn::chooseAgent(std::string ip, int port){

    agentAddress = createIPv4Address(ip.c_str(), port);
}