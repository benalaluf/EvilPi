#pragma once
#include <map>
#include <string>
#include <functional>


struct Command {
    std::string description;
    std::function<void()> function;
};

class CommandProcessor {
private:
    std::map<std::string, Command> commands;

public:
    void addCommand(const std::string& name, const std::string& description, const std::function<void()>& function);

    void processCommand(const std::string& command);

    void displayCommands() const;

};

