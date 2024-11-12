#include "SchedulerManager.h"
#include "../Process/Process.h"
#include "../Config/GlobalConfig.h"
#include "AScheduler.h"
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
    if (scheduler) {
        this->scheduler->execute();  // Execute the scheduling logic, processing one process at a time
    }
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

            if (!this->scheduler) {
                String schedulingAlgo = GlobalConfig::getInstance()->getScheduler();

				if (schedulingAlgo == "fcfs") {
					this->scheduler = std::make_shared<FCFSScheduler>(id, pname);
				}
				else if (schedulingAlgo == "rr") {
					//this->scheduler = std::make_shared<RRScheduler>(AScheduler::SchedulingAlgorithm::rr, id, pname);
				}
			}

			this->scheduler->addProcess(newUniqueProcess);
			return newUniqueProcess;
		}
        else {
            Process::RequirementFlags reqFlags = { true, 1, true, 1 };

            std::shared_ptr<Process> newProcess = std::make_shared<Process>(id, name, reqFlags);
            newProcess->generateRandomCommands();

			if (!this->scheduler) {
				String schedulingAlgo = GlobalConfig::getInstance()->getScheduler();

				if (schedulingAlgo == "fcfs") {
					this->scheduler = std::make_shared<FCFSScheduler>(id, name);
				}
				else if (schedulingAlgo == "rr") {
					//this->scheduler = std::make_shared<RRScheduler>(AScheduler::SchedulingAlgorithm::rr, id, name);
				}
			}

            this->scheduler->addProcess(newProcess);

            return newProcess;
        }
    }
}


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
    this->scheduler = nullptr;  // Initialize the scheduler as null, it will be created later
}