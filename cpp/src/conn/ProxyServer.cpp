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
    connections.reserve(2);

    bindServerSocket();
};

int ProxyServer::main() {

    std::thread thread1(&ProxyServer::acceptConnections, this);
    thread1.join();
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
        case (AGENTCONNECT): {
            connections.push_back(connData);
            std::thread thread1(&ProxyServer::handleAgent, this, connData);
            thread1.detach();
            break;
        }
        case (ADMINCONNECT): {
            connections.push_back(connData);
            std::thread thread1(&ProxyServer::handleAdmin, this, connData);
            thread1.detach();
            break;
        }
        default: {
            printf("cant handle this connectoin: %d", registerPacket.getType());
            close(connData.SockFD);
        }
    }

}

void ProxyServer::handleAgent(ConnData connData) {
    printf("handling agent\n");
    while (true) {
        Packet packet;
        int status = recvPacket(connData.SockFD, &packet);
        if (status == -1) handleConnectionClosed(connData);
        if (status!=0) break;
        if (memcmp(&packet.header.dst, address, sizeof(struct sockaddr_in)) != 0) {
            forward(packet);
        } else {
            handleAgentPacket(connData, packet);
        }
    }
}

void ProxyServer::handleAdmin(ConnData connData) {
    printf("handling adminssdf\n");
    while (true) {
        Packet packet;
        int status = recvPacket(connData.SockFD, &packet);
        if (status == -1) handleConnectionClosed(connData);
        if (status!=0) break;
        if (memcmp(&packet.header.dst, address, sizeof(struct sockaddr_in)) != 0) {
            forward(packet);
        } else {
            handleAdminPacket(connData, packet);
        }
    }

};

void ProxyServer::handleAgentPacket(ConnData connData ,Packet packet) {
    switch (packet.getType()) {
        case MSG: {
            MsgData msgData(packet.data, packet.getDataLength());
            printf("MSG FROM AGENT: %s\n", msgData.msg.c_str());
            break;
        }
        case RSH_COMMAND:
            break;
        case PING:
            break;
        case SETCONFFILE:
            break;
        case GETCONFFILE:
            break;
        case ERROR:
            exit(1);
    }
}

void ProxyServer::handleAdminPacket(ConnData connData, Packet packet) {
    switch (packet.getType()) {
        case MSG:
            break;
        case RSH_COMMAND:
            break;
        case SHOW:
            sendConnectionList(connData);
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
    for (int i = 0; i < connections.size(); i++) {
        if (memcmp(&connections[i].address.address, &packet.header.dst, sizeof(struct sockaddr_in)) == 0) {
            sendPacket(connections[i].SockFD, packet);
            printf("Forwarding to %s:%d\n", connections[i].address.connIP, connections[i].address.connPort);
        };
    }
}

void ProxyServer::handleConnectionClosed(ConnData connData) {
    std::cout << "dissconnected: " << connData.getData() << "\n";
    for (auto it = connections.begin(); it != connections.end(); ++it) {
        if (it->SockFD == connData.SockFD) {
            connections.erase(it);
            break;
        }
    }
}

void ProxyServer::sendConnectionList(ConnData connData) {
    std::string list;
    for (auto conn: connections) {
        list += conn.getData();
        list += "\n";
    }


    if (!list.empty()) {
        list.pop_back();
    }

    MsgData data(list);
    Packet p(SHOW, address, &connData.address.address, data);
    sendPacket(connData.SockFD, p);
}

