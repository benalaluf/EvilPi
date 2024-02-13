#pragma once

#include <cstddef>
#include <cstdint>
#include "Packet.h"

void printSerializedPacketMemory(uint8_t *data);

void printPacket(Packet &packet);