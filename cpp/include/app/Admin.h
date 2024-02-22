#pragma once
#include "conn/AdminConn.h"
#include "tui/AdminTUI.h"

class Admin{
public:
    Admin(const char *ip, int port);
    void main();

private:
    AdminConn *adminConn;
    AdminTUI *adminTui;

};
