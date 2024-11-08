#include "SchedulerWorker.h"			// CPU Core Worker
#include "../Scheduler/GlobalScheduler.h"

void SchedulerWorker::update(bool isRunning)
{
	this->isRunning = isRunning;
}

void SchedulerWorker::run()
{
	while (this->isRunning)
	{
		GlobalScheduler::getInstance()->tick();		// Checks the ready que and runs/assigned the next process

		std::cout << "Tick called.\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Small delay to reduce CPU usage
	}
}
