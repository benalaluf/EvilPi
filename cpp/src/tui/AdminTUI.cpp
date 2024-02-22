#include "tui/AdminTUI.h"
#include <iostream>


using namespace std::placeholders;
using std::cout, std::cin, std::getline;
using std::string;

AdminTUI::AdminTUI(AdminConn *adminConn) : adminConn(adminConn) {

    commandProcessor.addCommand("test", "for testing", std::bind(&AdminTUI::test, this, _1));
    commandProcessor.addCommand("choose", "for choosin", std::bind(&AdminTUI::choose, this, _1));

}

void AdminTUI::test(std::vector<std::string> args) {
    adminConn->test();

    for (std::string str: args) {
        std::cout << str << '\n';
    }
}

void AdminTUI::choose(std::vector<std::string> args) {

    string input;
    string ip;
    int port;

    cout << "enter ip: ";
    getline(cin, ip);
    cout << "enter port: ";
    getline(cin, input);

    port = std::stoi(input);

    adminConn->chooseAgent(ip, port);
}

void AdminTUI::run() {
    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "help") {
            commandProcessor.displayCommands();
        } else {
            commandProcessor.processCommand(input);
        }
    }
}



