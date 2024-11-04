#include "RoundRobinScheduler.h"

void RoundRobinScheduler::init()
{
	// Initialize the process queue
	processQueue = std::queue<std::shared_ptr<Process>>();

	// Clear the process queue
	while (!processQueue.empty())
	{
		processQueue.pop();
	}

	currentCycle = 0;
}

void RoundRobinScheduler::execute()
{
	// Check if there are processes in the queue
	if (!processQueue.empty())
	{
		auto currentProcess = processQueue.front();

		if (currentProcess->isFinished()) {
			processQueue.pop();
		}
		else {
			if (currentCycle < timeQuantum) {
				currentProcess->executeCurrentCommand();
				currentProcess->moveToNextLine();
				currentCycle++;
			}
			else {
				// Time slice expired, move to the next process
				processQueue.pop();
				processQueue.push(currentProcess);
				/*processQueue.pop();*/
				currentCycle = 0;
				
			}
		}

		/*currentCycle++;
		if (currentCycle >= timeQuantum) {
			currentCycle = 0;
			processQueue.push(currentProcess);
			processQueue.pop();
		}*/
	}
}