
#include "protocol/SocketUtil.h"
#include "protocol/Packet.h"
#include "protocol/PacketData.h"
#include "protocol/ProtocolDebug.h"
#include "utils/CommandParser.h"
#include "app/Admin.h"

#include <iostream>

#include <map>
#include <iostream>
#include <functional> // Include for std::function


int main() {


    Admin admin("0.0.0.0", 6969);
    admin.main();


    return 0;
}


