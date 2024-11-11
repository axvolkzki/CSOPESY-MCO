#include "AScheduler.h"

AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, String processName) : algorithm(schedulingAlgo)
{
	// Initialize the scheduler
	currentProcessInfo.pid = pid;
	currentProcessInfo.name = processName;
	currentProcessInfo.cpuID = 0;
	currentProcessInfo.lineCounter = 0;
	currentProcessInfo.linesOfCode = 0;
	currentProcessInfo.remainingTime = 0;
}

void AScheduler::addProcess(std::shared_ptr<Process> process)
{
	// Add process to the process map
	processMap[process->getName()] = process;

}

std::shared_ptr<Process> AScheduler::findProcess(String processName)
{
	auto it = processMap.find(processName);
	if (it != processMap.end()) {
		return it->second;
	} else {
		return nullptr;
	}
}

void AScheduler::run()
{
	// Ensure scheduler is initialized
	init();
}

void AScheduler::stop()
{
	isRunning = false;
}
