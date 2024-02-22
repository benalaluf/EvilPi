#include "utils/CommandParser.h"
#include <iostream>

void CommandProcessor::addCommand(const std::string &name, const std::string &description,
                                  const std::function<void()> &function) {
    commands[name] = {description, function};

}

void CommandProcessor::processCommand(const std::string &command) {
    auto it = commands.find(command);
    if (it != commands.end()) {
        it->second.function();
    } else {
        std::cout << "Unknown command. Please try again." << std::endl;
    }

}

void CommandProcessor::displayCommands() const {
    std::cout << "Available commands:\n";
    for (const auto& pair : commands) {
        std::cout << pair.first << ": " << pair.second.description << std::endl;
    }
}
