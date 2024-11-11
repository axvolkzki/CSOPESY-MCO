#pragma once
#include "AScheduler.h"
#include <queue>

class FCFSScheduler : public AScheduler
{
public:
	FCFSScheduler() : AScheduler(fcfs, 0, "") {}

    void init() override;  // Initialize FCFS specific settings
    void execute() override;  // Execute FCFS scheduling logic

	void addProcess(std::shared_ptr<Process> process);  // Adds a process to the queue

	//get process map
	std::unordered_map<String, std::shared_ptr<Process>> getProcessMap() const { return processMap; }

	
private:
	std::queue<std::shared_ptr<Process>> processQueue;  // Queue to store processes

	// Process map
	std::unordered_map<String, std::shared_ptr<Process>> processMap;
};

