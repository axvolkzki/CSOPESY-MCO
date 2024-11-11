#include "SchedulerManager.h"
#include "../Process/Process.h"
#include "../Config/GlobalConfig.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"

SchedulerManager* SchedulerManager::sharedInstance = nullptr;
SchedulerManager* SchedulerManager::getInstance()
{
    return sharedInstance;
}

void SchedulerManager::initialize()
{
    sharedInstance = new SchedulerManager();
}

void SchedulerManager::destroy()
{
    delete sharedInstance;
}

void SchedulerManager::tick()
{
    this->scheduler->execute();
}

std::shared_ptr<Process> SchedulerManager::createUniqueProcess(String name, int id)
{
    std::shared_ptr<Process> existingProcess = this->findProcess(name);

    if (existingProcess != nullptr) {
        return existingProcess;
    }
    else {
		if (name == "") {
			String pname = generateUniqueProcessName(id);

            Process::RequirementFlags reqFlags = { true, 1, true, 1 };

            std::shared_ptr<Process> newUniqueProcess = std::make_shared<Process>(id, pname, reqFlags);
            newUniqueProcess->generateRandomCommands();

            this->scheduler->addProcess(newUniqueProcess);

			return newUniqueProcess;
		}
        else {
            Process::RequirementFlags reqFlags = { true, 1, true, 1 };

            std::shared_ptr<Process> newProcess = std::make_shared<Process>(id, name, reqFlags);
            newProcess->generateRandomCommands();

            this->scheduler->addProcess(newProcess);

            return newProcess;
        }
    }
}

std::unordered_map<String, std::shared_ptr<Process>> SchedulerManager::getAllProcesses() const {
    return this->scheduler->getProcessMap();  // Access processMap from the scheduler instance
}

//std::vector<std::shared_ptr<Process>> SchedulerManager::getAllProcesses() const {
//    std::vector<std::shared_ptr<Process>> processList;
//    for (const auto& entry : this->scheduler->getProcessMap()) {  // Assuming getProcessMap() returns a map of all processes
//        processList.push_back(entry.second);
//    }
//    return processList;
//}


std::shared_ptr<Process> SchedulerManager::findProcess(String processName)
{
    return this->scheduler->findProcess(processName);
}

String SchedulerManager::generateUniqueProcessName(int id)
{
    std::stringstream ss;
    ss << "P" << id;
    return ss.str();
}

// Constructor
SchedulerManager::SchedulerManager()
{
	// Initialize the scheduler
	if (GlobalConfig::getInstance()->getScheduler() == "fcfs") {
		this->scheduler = std::make_shared<FCFSScheduler>();
	}
	else if (GlobalConfig::getInstance()->getScheduler() == "rr") {
		this->scheduler = std::make_shared<RRScheduler>();
	}
}