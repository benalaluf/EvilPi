#pragma once

#include <string>
#include "protocol/Networking.h"
#include "cstring"

#pragma pack(1)


class PacketData {
public:
    uint64_t dataLength;

    virtual uint8_t *serialized() const = 0;
};

class MsgData : public PacketData {
public:
    char *msg;

    MsgData() {};

    MsgData(char *msg);

    MsgData(uint8_t *bytes, uint64_t dataLength);

    uint8_t *serialized() const override;

};


class CommandData : public PacketData {
public:
    std::string command;

    CommandData() {};

    CommandData(std::string command);

    CommandData(uint8_t *bytes, uint64_t dataLength);

    uint8_t *serialized() const override;

};