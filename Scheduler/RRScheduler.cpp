#include "RRScheduler.h"
#include "../Config/GlobalConfig.h"

void RRScheduler::init()
{
    timeSlice = GlobalConfig::getInstance()->getQuantumCycles();
    processQueue = std::queue<std::shared_ptr<Process>>();
    currentProcess = nullptr;

	// Initialize process map
	processMap = std::unordered_map<String, std::shared_ptr<Process>>();
}

void RRScheduler::execute() {
	std::cout << "Executing RR scheduling algorithm" << std::endl;
}


void RRScheduler::addProcess(std::shared_ptr<Process> process)
{
    processQueue.push(process);

	// Add process to the process map
	processMap[process->getName()] = process;
}