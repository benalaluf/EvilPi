#include "conn/ProxyServer.h"
#include "protocol/SocketUtil.h"
#include "protocol/ProtocolDebug.h"
#include <thread>
#include <iostream>
#include <functional>
#include <csignal>
#include <sys/select.h>
#include "protocol/PacketData.h"
#include <fcntl.h>


ProxyServer::ProxyServer(const char *ip, int port) {
    serverSocketFD = createTCPIPv4Socket();
    int keepAlive = 1;
    setsockopt(serverSocketFD, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof keepAlive);

    address = createIPv4Address(ip, port);

    bindServerSocket();
};

int ProxyServer::main() {

    std::thread thread1(&ProxyServer::acceptConnections, this);
    std::thread thread2(&ProxyServer::handleClients, this);

    thread1.join();
    thread2.join();


    return 0;
};


bool ProxyServer::bindServerSocket() {
    int result = bind(serverSocketFD, (sockaddr *) address, sizeof(struct sockaddr_in));
    if (result == 0) {
        printf("socket bound successfully\n");
        return true;
    } else {
        printf("error with socket bind %d\n", result);
        return false;
    }
};

void ProxyServer::acceptConnections() {
    listen(serverSocketFD, 10);

    printf("listening...\n");
    while (true) {
        ConnData connData = acceptConn(serverSocketFD);
        try {
            printf("got connection from %s:%d\n", connData.address.connIP, connData.address.connPort);
            std::thread thread(&ProxyServer::handleConnection, this, connData);
            thread.detach();

        } catch (const std::exception &e) {
            std::cerr << "Exception in handleClient: " << e.what() << std::endl;

        }

    }
}

void ProxyServer::handleConnection(ConnData connData) {
    Packet registerPacket;
    int status = recvPacket(connData.SockFD, &registerPacket);
    if (status != 0) return;
    switch (registerPacket.getType()) {
        case (CONNECT): {
            connectedClients.push_back(connData);
            break;
        }
        case (ADMINLOGIN): {
            std::thread thread1(&ProxyServer::handleAdmin, this, connData);
            thread1.detach();
            break;
        }
        case (ERROR): {
            printf("error\n");
            break;
        }
        default: {
            printf("cant handle packet - %d", registerPacket.getType());
            close(connData.SockFD);
        }
    }

}

void ProxyServer::handleAdmin(ConnData connData) {
    printf("handling admin\n");
    adminConn = connData;
    while (true) {
        Packet packet;
        int status = recvPacket(connData.SockFD, &packet);
        printf("admin recv status %d\n", status);
        if (status != 0) break;
        if (memcmp(&packet.header.dst, address, sizeof(struct sockaddr_in)) != 0) {
            forward(packet);
        } else {
            handleAdminPacket(packet);
        }
    }
};

void ProxyServer::handleAdminPacket(Packet packet) {
    switch (packet.getType()) {
        case CONNECT:
            break;
        case MSG:
            break;
        case RSH:
            break;
        case SHOW:
            updateList();
            break;
        case PING:
            break;
        case ADMINLOGIN:
            break;
        case SETCONFFILE:
            break;
        case GETCONFFILE:
            break;
        case ERROR:
            exit(1);
    }

}

void ProxyServer::forward(Packet packet) {
    printf("forward method ");
    if (memcmp(&adminConn.address.address, &packet.header.dst, sizeof(struct sockaddr_in)) == 0) {
        sendPacket(adminConn.SockFD, packet);
        printf("Admin at %s:%d", adminConn.address.connIP, adminConn.address.connPort);
    }
    for (int i = 0; i < connectedClients.size(); i++) {
        if (memcmp(&connectedClients[i].address.address, &packet.header.dst, sizeof(struct sockaddr_in)) == 0) {
            sendPacket(connectedClients[i].SockFD, packet);
            printf("Client at %s:%d\n", connectedClients[i].address.connIP, connectedClients[i].address.connPort);
        };
    }
}

void ProxyServer::updateList() {
    char buffer[1024];
    for (int i = 0; i < connectedClients.size(); i++) {
        printf("%d -- ", i);
        connectedClients[i].print();
    }
//    for (int i = 0; i < connectedClients.size(); i++) {
//        if (isConnOpen(connectedClients[i])) {
//            connectedClients.erase(connectedClients.begin() + i);
//        }
//    }

}

// ...

void setSocketNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

// ...

void ProxyServer::handleClients() {
    printf("handling clients!!!!!!!!!!\n");
    while (true) {
        fd_set read_fds;
        int max_fd = -1;

        FD_ZERO(&read_fds);

        for (const ConnData &client: connectedClients) {
            if (client.SockFD != -1) {
                FD_SET(client.SockFD, &read_fds);
                if (client.SockFD > max_fd) {
                    max_fd = client.SockFD;
                }
            }
        }



        if (!connectedClients.empty()) {
            printf("got here\n");
            if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
                perror("select");
                exit(EXIT_FAILURE);
            }

            for (ConnData &client: connectedClients) {
                if (client.SockFD != -1 && FD_ISSET(client.SockFD, &read_fds)) {
                    Packet packet;
                    int result = recvPacket(client.SockFD, &packet);

                    if (result <= 0) {
                        if (result == 0) {
                            // Handle client disconnection logic if needed
                            printf("Client disconnected on sockfd %d\n", client.SockFD);
                            close(client.SockFD);
                            client.SockFD = -1;
                        } else {
                            perror("recv");
                            exit(EXIT_FAILURE);
                        }
                    } else {
                        if (memcmp(&packet.header.dst, address, sizeof(struct sockaddr_in)) != 0) {
                            forward(packet);
                        } else {
                            handleClientPacket(packet);
                        }
                    }
                }
            }
        }
    }
}


void ProxyServer::handleClientPacket(Packet packet) {
    printf("got packet from client?");
}

