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

	static void initialize();
	static void startAllCPUs();
	static void stopAllCPUs();
	static void destroy();

private:
	std::vector<std::shared_ptr<SchedulerWorker>> cpuCores;
	static ResourceEmulator* instance;
};

