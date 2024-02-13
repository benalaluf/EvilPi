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
