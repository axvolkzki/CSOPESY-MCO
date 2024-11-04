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
	}
}
