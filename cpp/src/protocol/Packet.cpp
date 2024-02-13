#include "protocol/Packet.h"

#include <iostream>

PacketHeader::PacketHeader(uint8_t *bytes) {
    int offset = 0;

    memcpy(&type, bytes, sizeof type);
    offset += sizeof type;

    memcpy(&dataLength, bytes + offset, sizeof dataLength);
    offset += sizeof dataLength;

    memcpy(&src, bytes + offset, sizeof src);
    offset += sizeof src;

    memcpy(&dst, bytes + offset, sizeof dst);
}

uint8_t *PacketHeader::serialized() {
    uint8_t *bytes = new uint8_t[PACKET_HEADER_LENGHT];
    int offset = 0;

    memcpy(bytes, &type, sizeof type);
    offset += sizeof type;

    memcpy(bytes + offset, &dataLength, sizeof dataLength);
    offset += sizeof dataLength;

    memcpy(bytes + offset, &src, sizeof src);
    offset += sizeof src;

    memcpy(bytes + offset, &dst, sizeof dst);

    return bytes;
}

uint64_t PacketHeader::getDataLength() {
    return be64toh(dataLength);;
}

PacketType PacketHeader::getType() {
    return (PacketType) be16toh(type);
}

Packet::~Packet() {
}

Packet::Packet(PacketType type, struct sockaddr_in *src, struct sockaddr_in *dst, void *data, uint64_t dataLength) {
    this->header.type = htobe16((uint16_t) type);
    this->header.dataLength = htobe64(dataLength);
    memcpy(&this->header.src, src, sizeof(struct sockaddr_in));
    memcpy(&this->header.dst, dst, sizeof(struct sockaddr_in));
    this->data = reinterpret_cast<uint8_t *>(data);
}

Packet::Packet(PacketType type, struct sockaddr_in *src, struct sockaddr_in *dst) :
        Packet(type, src, dst, nullptr, 0){};

Packet::Packet(PacketHeader header, uint8_t *bytes) {
    this->header = header;
    this->data = bytes;
}

Packet::Packet(uint8_t *bytes) {
    memcpy(&this->header.type, bytes, sizeof(this->header.type));
    memcpy(&this->header.dataLength, bytes + sizeof(this->header.type), sizeof(this->header.dataLength));

    uint8_t *packetData = new uint8_t[this->getDataLength()];
    memcpy(packetData, bytes + sizeof(struct PacketHeader), this->getDataLength());

    this->data = packetData;
}

uint8_t *Packet::seriallize() {
    int offset = 0;
    uint8_t *bytes = new uint8_t[this->getPacketLength()];
    memcpy(bytes, &this->header, PACKET_HEADER_LENGHT);
    offset += PACKET_HEADER_LENGHT;

    memcpy(bytes + offset, this->data, this->getDataLength());
    return bytes;
}

PacketType Packet::getType() {
    return (PacketType) be16toh(header.type);
}

uint64_t Packet::getDataLength() {
    return be64toh(header.dataLength);
}

const uint8_t *Packet::getData() {
    return data;
}


size_t Packet::getPacketLength() {
    return sizeof(struct PacketHeader) + this->getDataLength();
}

int sendPacket(int sockFD, Packet &&packet) {
    uint8_t *packetBytes = packet.seriallize();

    int result = send(sockFD, packetBytes, packet.getPacketLength(), 0);
    return result;
};

void switchPacketSrcDst(Packet &packet){
    struct sockaddr_in src = packet.header.src;
    packet.header.src = packet.header.dst;
    packet.header.dst = src;
}

int sendPacket(int sockFD, Packet &packet) {
    uint8_t *packetBytes = packet.seriallize();

    int result = send(sockFD, packetBytes, packet.getPacketLength(), 0);
    return result;
};

int recvPacket(int sockFD, Packet *receivedPacket) {
    PacketHeader packetHeader;
    if (recvall(sockFD, &packetHeader, PACKET_HEADER_LENGHT) != 0) {
        return -3;
    }

    receivedPacket->header = packetHeader;

    uint64_t dataLength = packetHeader.getDataLength();

    if (dataLength != 0) {
        uint8_t *databuffer = new uint8_t[dataLength];

        if (recvall(sockFD, databuffer, dataLength) != 0) {
            delete[] databuffer;
            return -4;
        }

        receivedPacket->data = databuffer;
    } else {
        receivedPacket->data = nullptr;
    }

    return 1;
}

int recvall(int sockFD, void *buffer, uint64_t dataLength) {
    uint64_t total = 0;
    ssize_t dataRecved;

    while (total < dataLength) {
        dataRecved = recv(sockFD, (uint8_t *) buffer + total, dataLength - total, 0);

        if (dataRecved == 0) {
            printf("Connection closed by peer\n");
            return -1;
        }

        if (dataRecved < 0) {
            return -2;
        }

        total += dataRecved;
    }

    return 0;
}
