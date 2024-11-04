#pragma once
#include "AScheduler.h"
#include <queue>


class RoundRobinScheduler :
    public AScheduler
{
public:
	RoundRobinScheduler(int pid, String processName, int timeQuantum) : AScheduler(SchedulingAlgorithm::ROUND_ROBIN, pid, processName), timeQuantum(timeQuantum) { }

	// Overridden functions
	void init() override;
	void execute() override;

private:
	int timeQuantum;
	int currentCycle = 0;
	std::queue<std::shared_ptr<Process>> processQueue;
};
