#include "tui/AdminTUI.h"
#include <iostream>




AdminTUI::AdminTUI(AdminConn *adminConn){

    commandProcessor.addCommand("test","for testing", std::bind(&AdminConn::test, adminConn));

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



