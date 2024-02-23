#include "app/Admin.h"
#include "conn/AdminConn.h"
#include "tui/AdminTUI.h"


Admin::Admin(const char *ip, int port){
    adminConn = new AdminConn(ip, port);
    adminTui = new AdminTUI(adminConn);
}

void Admin::main() {
    adminConn->main();
    adminTui->run();
}
