#include "conn/AdminConn.h"
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
        Packet packet(ADMINLOGIN,localAddress,serverAddress);
        sendPacket(adminSocketFD, packet);
        printf("CONNECTED TO SERVER\n");
    } else {
        printf("ERRRO WHILE TRYING TO CONNECT TO SEVER %d\n", result);
        exit(1);
    }
}

void AdminConn::adminInput() {
    char buffer[ADMIN_INPUT_BUFFER];


}

void AdminConn::handleAdminInput(char *buffer) {

}

void AdminConn::receive() {

}

void AdminConn::send() {

}

void AdminConn::handlePacket(Packet packet) {

}
