#pragma once
#include "../TypedefRepo.h"
#include "AScheduler.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"
#include <memory>
#include <mutex>


class SchedulerManager
{
public:
	static SchedulerManager* getInstance();						// Singleton instance
	static void initialize();									// Initialize the scheduler manager	
	static void destroy();										// Destroy the scheduler manager

	void tick();												// Tick the scheduler manager; called by SchedulerWorker

	std::shared_ptr<Process> createUniqueProcess(String name, int id);		// Create add a new process
	//std::vector<std::shared_ptr<Process>> getAllProcesses() const;

private:
	SchedulerManager();											// Constructor
	~SchedulerManager() = default;								// Default destructor

	SchedulerManager(SchedulerManager const&) {};				// Copy constructor
	SchedulerManager& operator=(SchedulerManager const&) {
		return *this;
	};															// Assignment operator
	static SchedulerManager* sharedInstance;					// Singleton instance
	
	std::shared_ptr<AScheduler> scheduler;						// Pointer to the scheduler
	std::mutex schedulerMutex;									// Scheduler mutex; for thread safety

	std::shared_ptr<Process> findProcess(String processName);	// Find a process by name
	String generateUniqueProcessName(int id);					// Generate a unique process name
};

