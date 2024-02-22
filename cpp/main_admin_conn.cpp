
#include "protocol/SocketUtil.h"
#include "protocol/Packet.h"
#include "protocol/PacketData.h"
#include "protocol/ProtocolDebug.h"
#include "utils/CommandParser.h"

#include <iostream>

#include <map>
#include <iostream>
#include <functional> // Include for std::function


int main() {
    CommandProcessor processor;

    // Adding a function using a lambda
    processor.addCommand("printHello","sdf" ,[]() {
        std::cout << "Hello, world!" << std::endl;
    });



    // Execute the stored functions
    processor.processCommand("printHello");


    return 0;
}


