#include "conn/ProxyServer.h"


int main() {
    ProxyServer server("0.0.0.0", 3211);
    server.main();
}