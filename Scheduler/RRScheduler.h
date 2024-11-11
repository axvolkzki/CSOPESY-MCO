#pragma once
#include "AScheduler.h"
#include "../TypedefRepo.h"
#include <queue>

class RRScheduler : public AScheduler {
public:
    RRScheduler(int timeSlice = 100) : AScheduler(rr, 0, ""), timeSlice(timeSlice) {}

    void init() override;         // Initialize RR-specific settings
    void execute() override;      // Execute RR scheduling algorithm

	void addProcess(std::shared_ptr<Process> process);  // Add process to the queue

	// Get process map
	std::unordered_map<String, std::shared_ptr<Process>> getProcessMap() const { return processMap; }

private:
    int timeSlice;  // Time slice for RR
    std::queue<std::shared_ptr<Process>> processQueue; // Queue for managing processes
    std::shared_ptr<Process> currentProcess;  // Process being executed

	// Process map
	std::unordered_map<String, std::shared_ptr<Process>> processMap;
};