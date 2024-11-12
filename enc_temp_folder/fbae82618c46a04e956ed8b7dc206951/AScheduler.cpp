#include "AScheduler.h"

AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, String processName) : algorithm(schedulingAlgo), isRunning(false)
{
}

void AScheduler::addProcess(std::shared_ptr<Process> process)
{
	readyQueue.push(process);  // Add process to the queue
}

std::shared_ptr<Process> AScheduler::findProcess(String processName)
{
	// Find the process by name in the queue
	std::queue<std::shared_ptr<Process>> tempQueue = readyQueue;
	while (!tempQueue.empty()) {
		std::shared_ptr<Process> process = tempQueue.front();
		if (process->getName() == processName) {
			return process;
		}
		tempQueue.pop();
	}
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
