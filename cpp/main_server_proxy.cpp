#include "conn/ProxyServer.h"


int main() {
    ProxyServer server("0.0.0.0", 8080);
    server.main();
}