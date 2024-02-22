#pragma once
#include <map>
#include <string>
#include <functional>
#include <vector>


struct Command {
    std::string description;
    std::function<void(std::vector<std::string>)> function;
};

class CommandProcessor {
private:
    std::map<std::string, Command> commands;

public:
    void addCommand(const std::string& name, const std::string& description,
                    const std::function<void(std::vector<std::string>)>& function);

    void processCommand(const std::string& command);

    std::vector<std::string> parseCommand(const std::string& str);

    void displayCommands() const;

};

