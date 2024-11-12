#include <iostream>
#include <random>
#include "Process.h"
#include "../TypedefRepo.h"
#include "../Console/MainConsole.h"
#include "../Config/GlobalConfig.h"
#include "../Command/ICommand.h"
#include "../Command/PrintCommand.h"

Process::Process(int pid, String name, RequirementFlags requirementFlags)
    : pid(pid), name(name), requirementFlags(requirementFlags), currentState(READY), commandCounter(0) {
}

void Process::addCommand(ICommand::CommandType commandType) {
    if (commandType == ICommand::PRINT) {
        String toPrint = "Executing command for PID " + std::to_string(this->pid);
        auto printCommand = std::make_shared<PrintCommand>(this->pid, toPrint);
        this->commandList.push_back(printCommand);
    } else if (commandType == ICommand::IO) {
        // Placeholder for future I/O command handling, if needed.
    }
}

void Process::generateRandomCommands() {
    int limit = GlobalConfig::getInstance()->getRandomInstructionCount();
    for (int i = 0; i < limit; i++) {
        addCommand(ICommand::PRINT);  // Adds PRINT commands based on configured instruction count.
    }
}

void Process::executeCurrentCommand() {
    std::lock_guard<std::mutex> lock(processMutex);
    if (commandCounter < commandList.size()) {
        commandList[commandCounter]->execute();  // Execute the command at the current counter.
        std::cout << "Process " << pid << " executed command at line " << commandCounter << std::endl;
        commandCounter++;
    } else {
        std::cerr << "Process " << pid << " has no more commands to execute." << std::endl;
        updateState(FINISHED);  // Mark as finished if all commands are executed.
    }
}

void Process::moveToNextLine() {
    std::lock_guard<std::mutex> lock(processMutex);
    if (commandCounter >= commandList.size()) {
        updateState(FINISHED);  // Update state if all commands are completed.
    } else {
        currentState = RUNNING;  // Continue running if there are commands left.
    }
}

void Process::updateState(const ProcessState newState) {
    std::lock_guard<std::mutex> lock(processMutex);
    currentState = newState;
}

bool Process::isFinished() const {
    return currentState == FINISHED;
}

int Process::getRemainingTime() const {
    std::lock_guard<std::mutex> lock(processMutex);
    return commandList.size() - commandCounter;
}

int Process::getCommandCounter() const {
    return commandCounter;
}

int Process::getLinesOfCode() const {
    return commandList.size();
}

int Process::getPID() const {
    return pid;
}

int Process::getCPUCoreID() const {
    return cpuCoreID;
}

Process::ProcessState Process::getState() const {
    std::lock_guard<std::mutex> lock(processMutex);
    return currentState;
}

String Process::getName() const {
    return name;
}
