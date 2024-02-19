#pragma once
#pragma pack(push, 1)

#include <sys/socket.h>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <string>
#include <memory>
#include <cstring>
#include "protocol/Networking.h"

#define HEADER_SIZE sizeof (struct PacketHeader)
#define PACKET_HEADER_LENGTH_LENGTH sizeof uint64_t
#define PACKET_HEADER_LENGTH_TYPE sizeof (uint16_t)
#define PACKET_HEADER_LENGHT sizeof (struct PacketHeader)

enum PacketType : uint16_t {
    CONNECT = 1,
    MSG = 2,
    RSH = 3,
    SHOW = 4,
    PING = 5,
    ADMINLOGIN = 69,
    SETCONFFILE = 200,
    GETCONFFILE = 2001,
    ERROR = 0,

};


struct PacketHeader {
    uint16_t type;
    uint64_t dataLength;
    struct sockaddr_in src;
    struct sockaddr_in dst;

    PacketHeader(){};

    PacketHeader(uint8_t *bytes);

    uint8_t *serialized();

    uint64_t getDataLength();

    PacketType getType();
};


struct Packet {
    PacketHeader header;
    uint8_t *data;;

    Packet(PacketType type, struct sockaddr_in *src, struct sockaddr_in *dst, void *data, uint64_t dataLength);

    Packet(PacketType type, struct sockaddr_in *src, struct sockaddr_in *dst);

    Packet(PacketHeader header, uint8_t *bytes);

    Packet(uint8_t *bytes);

    Packet() {};

    ~Packet();

    uint8_t *seriallize();

    PacketType getType();

    uint64_t getDataLength();

    const uint8_t *getData();

    size_t getPacketLength();

};
void switchPacketSrcDst(Packet &packet);
int sendPacket(int sockFD, Packet &&packet);

int sendPacket(int sockFD, Packet &packet);

int recvPacket(int sockFD, Packet *receivedPacket);

int recvall(int sockFD, void *buffer, uint64_t dataLength);

#pragma pack(pop)
