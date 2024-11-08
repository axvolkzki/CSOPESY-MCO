#include "RoundRobinScheduler.h"
#include "../Config/GlobalConfig.h"
#include "../Console/ConsoleManager.h"

void RoundRobinScheduler::init()
{
	// Initialize the process queue
	processReadyQueue = std::queue<std::shared_ptr<Process>>();

	// Get the time quantum from the configuration
	GlobalConfig& config = GlobalConfig::getInstance();
	timeQuantum = config.getQuantumCycles();
	currentCycle = 0;
}

void RoundRobinScheduler::execute()
{
    // Add processes from AScheduler's processMap to the ready queue
    for (auto& entry : processMap) {
        processReadyQueue.push(entry.second);  // Add process to the ready queue
    }

    if (!processReadyQueue.empty()) {
        auto currentProcess = processReadyQueue.front(); // Get the next process

        // Execute the current command of the process
        currentProcess->executeCurrentCommand();
        currentProcess->moveToNextLine();

        // Increment cycle count and check if time quantum is finished
        if (currentCycle >= timeQuantum) {
            processReadyQueue.push(processReadyQueue.front());  // Move process to the back
            processReadyQueue.pop();                            // Remove it from the front
            currentCycle = 0;                                   // Reset cycle for the next process
        } else {
            currentCycle++; // Increment cycle for current process
        }

        // If the process is finished, pop it from the queue
        if (currentProcess->isFinished()) {
            processReadyQueue.pop(); // Remove finished process
            currentCycle = 0;        // Reset cycle for the next process

            // Find the process and set its status to finished
            auto it = processMap.find(currentProcess->getName());
            
            // if (it != processMap.end()) {
            //     it->second->currentState = Process::FINISHED;
            // }
        }
    }
}
