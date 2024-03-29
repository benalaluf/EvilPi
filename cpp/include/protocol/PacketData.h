#pragma once

#include "protocol/Packet.h"
#include "protocol/Networking.h"
#include "cstring"

#pragma pack(1)


class PacketData {
public:
    uint64_t dataLength;

    virtual uint8_t *serialized() = 0;
};

class MsgData : public PacketData {
public:
    char *msg;

    MsgData() {};

    MsgData(char *msg);

    MsgData(uint8_t *bytes, uint64_t dataLength);//todo: fix this: ugly

    uint8_t *serialized() override;

};

