#pragma once
#include <memory>
#include <vector>
#include <mutex>
#include "../Command/ICommand.h"
#include "../TypedefRepo.h"

class Process {
public:
    struct RequirementFlags {
        bool requireFiles;
        int numFiles;
        bool requireMemory;
        int memoryRequired;
    };

    enum ProcessState {
        READY,
        RUNNING,
        WAITING,
        FINISHED
    };

    Process(int pid, String name, RequirementFlags requirementFlags);
    void addCommand(ICommand::CommandType commandType);
    void executeCurrentCommand();
    void moveToNextLine();

    void updateState(const ProcessState newState);
    bool isFinished() const;
    int getRemainingTime() const;
    int getCommandCounter() const;
    int getLinesOfCode() const;
    int getPID() const;
    int getCPUCoreID() const;
    ProcessState getState() const;
    String getName() const;

    void generateRandomCommands();  // Generate random commands for testing

private:
    int pid;
    String name;
    typedef std::vector<std::shared_ptr<ICommand>> CommandList;
    CommandList commandList;

    int commandCounter = 0;
    int cpuCoreID = -1;
    RequirementFlags requirementFlags;
    ProcessState currentState = READY;

    std::mutex processMutex;

    friend class ResourceEmulator;
};
