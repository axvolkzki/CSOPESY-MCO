#include "SchedulerWorker.h"			// CPU Core Worker

#include "../Scheduler/SchedulerManager.h"

void SchedulerWorker::update(bool isRunning)
{
	this->isRunning = isRunning;
}

void SchedulerWorker::run()
{
	while (this->isRunning)
	{
		SchedulerManager::getInstance()->tick();		// Checks the ready que and runs/assigned the next process
		//std::cout << "Tick called.\n";	// Debugging
	}
}
