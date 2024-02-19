#include "conn/ProxyServer.h"


int main() {
    ProxyServer server("0.0.0.0", 6969);
    server.main();
}