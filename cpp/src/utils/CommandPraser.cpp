#include "utils/CommandParser.h"
#include <iostream>
#include <sstream>

void CommandProcessor::addCommand(const std::string &name, const std::string &description,
                                  const std::function<void(std::vector<std::string>)> &function) {
    commands[name] = {description, function};

}

void CommandProcessor::processCommand(const std::string &command) {
    auto args= parseCommand(command);
    auto it = commands.find(args[0]);
    if (it != commands.end()) {
        it->second.function(args);
    } else {
        std::cout << "Unknown command. Please try again." << std::endl;
    }

}
std::vector<std::string> CommandProcessor::parseCommand(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void CommandProcessor::displayCommands() const {
    std::cout << "Available commands:\n";
    for (const auto& pair : commands) {
        std::cout << pair.first << ": " << pair.second.description << std::endl;
    }
}

