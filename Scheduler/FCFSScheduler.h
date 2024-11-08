#pragma once
#include "AScheduler.h"
#include <queue>


class FCFSScheduler :
    public AScheduler
{
public:
	FCFSScheduler(int pid, String processName) : AScheduler(SchedulingAlgorithm::FCFS, pid, processName) { }
	

	// Overridden functions
	void init() override;
	void execute() override;
private:
	std::queue<std::shared_ptr<Process>> processQueue;
};

