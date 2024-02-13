#include "protocol/SocketUtil.h"
#include "conn/ProxyServer.h"

int createTCPIPv4Socket() {
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    return socketFD;
}

struct sockaddr_in *createIPv4Address(const char *ip, int port) {
    struct sockaddr_in *address = new struct sockaddr_in;
    address->sin_family = AF_INET;
    address->sin_port = htons(port);

    if (strlen(ip) == 0)
        address->sin_addr.s_addr = INADDR_ANY;
    else
        inet_pton(AF_INET, ip, &address->sin_addr.s_addr);
    return address;
}

ConnData acceptConn(int sockFD){
    struct sockaddr_in address;
    socklen_t sizeOfAddress = sizeof(struct sockaddr_in);

    int connSockFD = accept(sockFD, (sockaddr *) &address, &sizeOfAddress);

    return {connSockFD, address, sizeOfAddress};
}

struct sockaddr_in* createIPv4Address(int sockFD) {
    struct sockaddr_in* addr = new struct sockaddr_in;
    socklen_t addrLen = sizeof(struct sockaddr_in);

    if (getsockname(sockFD, reinterpret_cast<struct sockaddr*>(addr), &addrLen) == -1) {
        perror("getsockname");
        close(sockFD);
        delete addr; // Free allocated memory before exiting
        exit(EXIT_FAILURE);
    }
    return addr;
}