#pragma once
#include "AScheduler.h"
#include "ResourceEmulator.h"

class FCFSScheduler : public AScheduler
{
public:
	FCFSScheduler(int pid, String processName) : AScheduler(SchedulingAlgorithm::fcfs, pid, processName) {}

    void init() override;  // Initialize FCFS specific settings
    void execute() override;  // Execute FCFS scheduling logic

private:
	// Map to track which process is assigned to each core (index in cpuCores)
	std::unordered_map<int, std::shared_ptr<Process>> coreProcessMap;

	bool assignToAvailableCore(const std::shared_ptr<Process>& process);

	bool isCoreFree(int coreIndex);
};

