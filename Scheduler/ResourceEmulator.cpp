#include "ResourceEmulator.h"
#include "../Config/GlobalConfig.h"
#include <memory>

ResourceEmulator* ResourceEmulator::instance = nullptr;

void ResourceEmulator::initialize()
{
	if (instance == nullptr)
	{
		instance = new ResourceEmulator();
		int numCores = GlobalConfig::getInstance()->getNumCPU();

		for (int i = 0; i < numCores; i++)
		{
			auto worker = std::make_shared<SchedulerWorker>();
			instance->cpuCores.push_back(worker);
		}
	}
}

void ResourceEmulator::startAllCPUs()
{
	for (auto& core : instance->cpuCores)
	{
		std::thread(&SchedulerWorker::run, core).detach(); // Start each core in a separate thread
	}
}

void ResourceEmulator::stopAllCPUs()
{
	for (auto& core : instance->cpuCores) {
		core->update(false);
	}
}

void ResourceEmulator::destroy()
{
	if (instance != nullptr)
	{
		instance->stopAllCPUs();
		instance->cpuCores.clear();
		delete instance;
		instance = nullptr;
	}
}

std::vector<std::shared_ptr<SchedulerWorker>> ResourceEmulator::getCPUCores() const
{
	return cpuCores;
}