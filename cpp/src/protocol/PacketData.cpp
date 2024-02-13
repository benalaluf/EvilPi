#include "protocol/PacketData.h"
#include "protocol/Networking.h"


MsgData::MsgData(char *msg) : msg(msg) {
    dataLength = strlen(msg);
}

MsgData::MsgData(uint8_t *bytes, uint64_t dataLength) {
    msg = new char[dataLength];
    memcpy(msg, bytes, dataLength);
}

uint8_t *MsgData::serialized() {
    uint8_t *bytes = new uint8_t[dataLength];
    memcpy(bytes, msg, dataLength);
    return bytes;
}


