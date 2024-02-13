#pragma once

#include "protocol/Networking.h"
#include <cstdio>
#include <string>
#include <iostream>

struct ConnAddress {
    struct sockaddr_in address;
    socklen_t sizeOfAddress = sizeof(struct sockaddr_in);

    char connIP[INET6_ADDRSTRLEN];
    unsigned short connPort;
    ConnAddress(){};
    ConnAddress(struct sockaddr_in addr, socklen_t size) {
        address = addr;
        sizeOfAddress = size;
        inet_ntop(AF_INET, &(address.sin_addr), connIP, INET_ADDRSTRLEN);
        connPort = ntohs(address.sin_port);
    };
};

struct ConnData {
    int SockFD;
    struct ConnAddress address;

    ConnData(){};

    ConnData(int sockFD, struct sockaddr_in addr, socklen_t size) : address(addr, size) {
        SockFD = sockFD;
    };

    void print() {
        printf("Client fd: %d, addr: %s:%d\n", SockFD, address.connIP, address.connPort);
    }

    char *getData() {
        char *data = new char[25];
        snprintf(data, 25, "Client fd: %d, addr: %s:%d\n", SockFD, address.connIP, address.connPort);
        return data;
    }


};

