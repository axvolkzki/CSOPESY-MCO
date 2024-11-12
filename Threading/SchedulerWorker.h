/**
* SchedulerWorker.h -- Header file for the SchedulerWorker class
* 
* This class is a worker class that simulates a CPU core. It is used by the ResourceEmulator class to simulate
* multiple CPU cores. The worker class runs in a separate thread and executes the tick() method of the SchedulerManager
* class. The tick() method checks the instructions of the processes and executes them accordingly.
* FCFS - First Come First Serve; all processes are executed in the order they arrive. the thread will not stop until all instructions are executed.
* RR - Round Robin; all processes are executed in a round-robin fashion. The thread will not stop until all instructions are executed.
*/

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

