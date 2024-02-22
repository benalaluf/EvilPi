#include "tui/AdminTUI.h"
#include <iostream>


using namespace std::placeholders;

AdminTUI::AdminTUI(AdminConn *adminConn):adminConn(adminConn){

    commandProcessor.addCommand("test","for testing", std::bind(&AdminTUI::test, this, _1));

}

void AdminTUI::test(std::vector<std::string> args){
    adminConn->test();

    for (std::string str: args){
        std::cout << str << '\n';
    }
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



