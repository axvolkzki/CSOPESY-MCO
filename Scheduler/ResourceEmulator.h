#pragma once
#include <vector>
#include "../Threading/SchedulerWorker.h"
#include <memory>

class ResourceEmulator
{
public:
	static ResourceEmulator* getInstance() {
		if (instance == nullptr) {
			instance = new ResourceEmulator();
		}
		return instance;
	}

	static void initialize();			// Initialize the resource emulator
	static void startAllCPUs();			// Start all CPUs (SchedulerWorker threads)
	static void stopAllCPUs();			// Stop all CPUs (SchedulerWorker threads)
	static void destroy();				// Destroy the resource emulator; cleans ip resources

	std::vector<std::shared_ptr<SchedulerWorker>> getCPUCores() const;	// Get a list of CPU cores (SchedulerWorkers)

private:
	std::vector<std::shared_ptr<SchedulerWorker>> cpuCores;	// a list of CPU cores (SchedulerWorkers)
	static ResourceEmulator* instance;						// Singleton instance
};

