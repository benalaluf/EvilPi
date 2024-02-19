#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include "Packet.h"

void printSerializedPacketMemory(uint8_t *data);

void printPacket(Packet &packet);

template<typename T>
void print_memory(const T& value) {
    const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&value);
    std::cout << "Memory representation of " << value << " (size " << sizeof(value) << " bytes):" << std::endl;
    for (size_t i = 0; i < sizeof(value); ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ptr[i]) << " ";
    }
    std::cout << std::dec << std::endl;
}


void printPacketHeader(const PacketHeader& header);