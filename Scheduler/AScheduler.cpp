#include "AScheduler.h"

AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, String processName) : algorithm(schedulingAlgo), isRunning(false)
{
}

void AScheduler::addProcess(std::shared_ptr<Process> process)
{
	processes.emplace_back(process);
}

std::shared_ptr<Process> AScheduler::findProcess(String processName)
{
	// find the process using name
	
}

void AScheduler::run()
{
	isRunning = true;
	init();
	while (isRunning) {
		execute();  // Run scheduling logic (to be implemented in derived class)
	}
}

void AScheduler::stop()
{
	isRunning = false;
}
