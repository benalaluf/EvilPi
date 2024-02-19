#include "protocol/ProtocolDebug.h"

void printSerializedPacketMemory(uint8_t *data) {
    Packet packet(data);
    printf("Memory layout of Packet:\n");
    printf("--------------------------------\n");

    for (size_t i = 0; i < packet.getPacketLength(); ++i) {
        printf("%02x ", data[i]);
    }

    printf("\n");
}


void printPacket(Packet &packet){
    printf("Packet: \n");
    printf("--------------------------------\n");
    printf("type: %d\nbytes-dataLength: %d\n", packet.getType(), packet.getDataLength());
    printf("bytes: %s", packet.getData());
};
void printPacketHeader(const PacketHeader& header) {
    std::cout << "Packet Header:" << std::endl;
    std::cout << "Type: " << header.getType() << std::endl;
    std::cout << "Data Length: " << header.getDataLength() << std::endl;
    char srcAddr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(header.src.sin_addr), srcAddr, INET_ADDRSTRLEN);
    std::cout << "Source Address: " << srcAddr << ":" << ntohs(header.src.sin_port) << std::endl;
    char dstAddr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(header.dst.sin_addr), dstAddr, INET_ADDRSTRLEN);
    std::cout << "Destination Address: " << dstAddr << ":" << ntohs(header.dst.sin_port) << std::endl;
}