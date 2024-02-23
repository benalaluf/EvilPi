#pragma once

#include "conn/AdminConn.h"
#include "utils/CommandParser.h"


class AdminTUI {
public:
    AdminTUI(AdminConn *adminConn);


    void run();
    void msgAgent(std::vector<std::string> args);

private:
    AdminConn *adminConn;
    CommandProcessor commandProcessor;


    void choose(std::vector<std::string> args);

    void showConnections(std::vector<std::string> args);
};
