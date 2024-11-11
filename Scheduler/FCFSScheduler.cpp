#include "FCFSScheduler.h"

void FCFSScheduler::init()
{
	// Initialize any necessary data for FCFS (e.g., process queue)
	processQueue = std::queue<std::shared_ptr<Process>>();

	// Initialize process map
	processMap = std::unordered_map<String, std::shared_ptr<Process>>();
}

void FCFSScheduler::execute() {
    // If there are processes in the queue, run the first one
    if (!processQueue.empty()) {
        std::shared_ptr<Process> currentProcess = processQueue.front();
        processQueue.pop();

        // Execute the process (for simplicity, let's assume it's a function like "run")
        std::cout << "Executing process: " << currentProcess->getName() << std::endl;
        currentProcess->updateState(Process::RUNNING);  // Set state to RUNNING
        currentProcess->executeCurrentCommand();
        currentProcess->moveToNextLine();

        // Check if the process is finished
        if (currentProcess->isFinished()) {
            currentProcess->updateState(Process::FINISHED);  // Set state to FINISHED
        }
        else {
            // If process is not finished, it should remain in the queue
            processQueue.push(currentProcess);  // Re-queue it to continue execution
        }
    }
}


void FCFSScheduler::addProcess(std::shared_ptr<Process> process)
{
    // FCFS simply adds processes to the queue in the order they arrive
    processQueue.push(process);

	// Add process to the process map
	processMap[process->getName()] = process;
}
