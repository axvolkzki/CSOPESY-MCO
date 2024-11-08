#include "AScheduler.h"

AScheduler::AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, String processName)
    : IETThread(), // Initialize base class (thread)
    schedulingAlgo(schedulingAlgo), pid(pid), processName(processName) {}

void AScheduler::addProcess(std::shared_ptr<Process> process) {
    // Add process to a storage (e.g., a queue or list depending on scheduler type)
    processMap[process->getName()] = process;
}

std::shared_ptr<Process> AScheduler::findProcess(String processName) {
    // Find and return process by name
    auto it = processMap.find(processName);
    if (it != processMap.end()) {
        return it->second;
    }
    return nullptr; // Return nullptr if not found
}

void AScheduler::run() {
    init();  // Initialize the scheduler
    while (running) { // Assume `running` is a flag to control the loop
        execute(); // Execute the scheduling policy
    }
}

void AScheduler::stop() {
    running = false; // Stop the scheduler loop
}


