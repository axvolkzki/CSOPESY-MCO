#include "FCFSScheduler.h"


void FCFSScheduler::init()
{
	// Initialize the process queue
	processQueue = std::queue<std::shared_ptr<Process>>();

	// Clear the process queue
	while (!processQueue.empty())
	{
		processQueue.pop();
	}
}

void FCFSScheduler::execute()
{
	if (processQueue.empty()) return;  // Check if queue is empty
    
    auto currentProcess = processQueue.front();

    if (currentProcess->isFinished()) {
        processQueue.pop();  // Remove the process if it's finished
    } else {
        currentProcess->executeCurrentCommand();
        currentProcess->moveToNextLine();
    }
}