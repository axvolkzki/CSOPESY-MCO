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
		SchedulerManager::getInstance()->tick(); // assign a process to the CPU core
	}
}
