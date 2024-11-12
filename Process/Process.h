// Process.h
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
	void executeCurrentCommand() const;
	void moveToNextLine();

	bool isFinished() const;
	int getRemainingTime() const;
	int getCommandCounter() const;
	int getLinesOfCode() const;
	int getPID() const;
	int getCPUCoreID() const;
	ProcessState getState() const;
	String getName() const;

	void setCPUCoreID(int coreID);
	void setState(ProcessState state);

	void generateRandomCommands();

private:
	int pid;
	String name;
	typedef std::vector<std::shared_ptr<ICommand>> CommandList;
	CommandList commandList;

	int commandCounter;
	int cpuCoreID = -1;
	RequirementFlags requirementFlags;
	ProcessState currentState;

	std::mutex processMutex;

	friend class ResourceEmulator;
};
