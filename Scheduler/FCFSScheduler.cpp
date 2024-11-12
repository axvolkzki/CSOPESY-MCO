#include "FCFSScheduler.h"

void FCFSScheduler::init()
{
    // Initialization logic, if any, specific to FCFS
    isRunning = true;

    for (const auto& process : processes) {
        int core = 0;

        if (core >= 0 && core < GlobalConfig::getInstance()->getNumCPU()) {
            processQueue[core].push_back(process);
        }
        else {
            std::cerr << "Invalid core!" << std::endl;
        }
    }
}


void FCFSScheduler::execute() {
	std::cout << "Executing FCFS scheduling algorithm" << std::endl;

    if (!isRunning || readyQueue.empty()) return;

    // Get the next process in the ready queue
    auto process = readyQueue.front();
    readyQueue.pop();  // Remove the process from the queue

    // Assign this process to a CPU core and start execution
    assignToAvailableCore(process);
}

bool FCFSScheduler::assignToAvailableCore(const std::shared_ptr<Process>& process) {
    auto* resourceEmulator = ResourceEmulator::getInstance();

    // Iterate over cpuCores to find an available (free) core
    for (size_t coreIndex = 0; coreIndex < resourceEmulator->getCPUCores().size(); ++coreIndex) {
        if (isCoreFree(coreIndex)) {
            // Assign process to this core
            coreProcessMap[coreIndex] = process;
			process->setCPUCoreID(coreIndex);
			process->setState(Process::RUNNING);

            // Start the process on this core's thread
            resourceEmulator->getCPUCores()[coreIndex]->run();
            return true;  // Process assigned successfully
        }
    }
    return false;  // No available core found
}

bool FCFSScheduler::isCoreFree(int coreIndex) {
    // Check if the core is currently unassigned or its assigned process has finished
    if (coreProcessMap[coreIndex] == nullptr) {
        return true;  // Core is free if no process is assigned
    }
    auto process = coreProcessMap[coreIndex];
    if (process->isFinished()) {
        // Reset core status and return true if the process on it has finished
        coreProcessMap[coreIndex] = nullptr;
        process->setState(Process::RUNNING);
        return true;
    }
    return false;
}
