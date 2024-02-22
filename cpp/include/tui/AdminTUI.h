#pragma once

#include "conn/AdminConn.h"
#include "utils/CommandParser.h"


class AdminTUI {
public:
    AdminTUI(AdminConn *adminConn);


    void run();

private:

    CommandProcessor commandProcessor;


};
