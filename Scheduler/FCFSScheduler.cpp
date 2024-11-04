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
	// Check if there are processes in the queue
	if (!processQueue.empty())
	{
		auto currentProcess = processQueue.front();

		if (currentProcess->isFinished()) {
			processQueue.pop();
		}
		else {
			currentProcess->executeCurrentCommand();
			currentProcess->moveToNextLine();
		}


		//// Get the first process in the queue
		//std::shared_ptr<Process> process = processQueue.front();
		//// Remove the process from the queue
		//processQueue.pop();
		//// Execute the process
		//process->execute();
	}
}