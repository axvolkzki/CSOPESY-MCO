#include "ResourceEmulator.h"
#include "../Config/GlobalConfig.h"
#include <memory>

ResourceEmulator* ResourceEmulator::instance = nullptr;

void ResourceEmulator::initialize()
{
	GlobalConfig config;

	if (instance == nullptr)
	{
		instance = new ResourceEmulator();
		int numCores = config.getNumCPU();

		for (int i = 0; i < numCores; i++)
		{
			auto worker = std::make_shared<SchedulerWorker>();
			instance->cpuCores.push_back(worker);
			//instance->cpuCores.push_back(std::make_shared<SchedulerWorker>());
		}
	}
}

void ResourceEmulator::startAllCPUs()
{
	for (auto& core : instance->cpuCores)
	{
		/*core->start();*/
		std::thread(&SchedulerWorker::run, core).detach(); // Start each core in a separate thread
	}
}

void ResourceEmulator::stopAllCPUs()
{
	for (auto& core : instance->cpuCores)
	{
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