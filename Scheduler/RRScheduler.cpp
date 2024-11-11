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
    if (currentProcess != nullptr) {
        // If current process still has time, continue executing
        currentProcess->updateState(Process::RUNNING);  // Set state to RUNNING
        currentProcess->executeCurrentCommand();
        currentProcess->moveToNextLine();

        // Check if process is finished after executing current command
        if (currentProcess->isFinished()) {
            currentProcess->updateState(Process::FINISHED);  // Set state to FINISHED
            std::cout << "Process " << currentProcess->getName() << " finished." << std::endl;
            currentProcess = nullptr;  // Reset current process
        }
        else {
            // If not finished, continue to next round
            processQueue.push(currentProcess);  // Re-queue the process
        }
    }

    // If there's no current process, pick the next one from the queue
    if (currentProcess == nullptr && !processQueue.empty()) {
        currentProcess = processQueue.front();
        processQueue.pop();
        std::cout << "Switching to process: " << currentProcess->getName() << std::endl;
    }
}


void RRScheduler::addProcess(std::shared_ptr<Process> process)
{
    processQueue.push(process);

	// Add process to the process map
	processMap[process->getName()] = process;
}