#include "string.h"
#include "protocol/PacketData.h"
#include "protocol/Networking.h"


MsgData::MsgData(char *msg) : msg(msg) {
    dataLength = strlen(msg);
}

MsgData::MsgData(uint8_t *bytes, uint64_t dataLength) {
    msg = new char[dataLength];
    memcpy(msg, bytes, dataLength);
}

uint8_t *MsgData::serialized() const {
    uint8_t *bytes = new uint8_t[dataLength];
    memcpy(bytes, msg, dataLength);
    return bytes;
}


CommandData::CommandData(std::string command) : command(command) {
    dataLength = command.size();
}

CommandData::CommandData(uint8_t *bytes, uint64_t dataLength) {
    command = std::string(reinterpret_cast<const char *>(bytes));
}

uint8_t *CommandData::serialized() const {
    uint8_t *bytes = new uint8_t[dataLength];
    memcpy(bytes, command.c_str(), dataLength);
    return bytes;
}



