#pragma once
#include "IETThread.h"

class SchedulerWorker : public IETThread
{
public:
	SchedulerWorker() = default;
	void update(bool isRunning);		// Update the running status of the worker
	void run() override;				// Thread's main execution logic

private:
	bool isRunning = true;				// Flag to control the worker's execution
};

