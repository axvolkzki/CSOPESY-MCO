#include "SchedulerManager.h"
#include "../Process/Process.h"
#include "../Config/GlobalConfig.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"
#include "../Memory/FlatMemoryAllocator.h"

SchedulerManager* SchedulerManager::sharedInstance = nullptr;

SchedulerManager* SchedulerManager::getInstance() {
    return sharedInstance;
}

void SchedulerManager::initialize() {
    sharedInstance = new SchedulerManager();
}

void SchedulerManager::destroy() {
    delete sharedInstance;
}

void SchedulerManager::tick() {
    std::lock_guard<std::mutex> lock(schedulerMutex);
    if (scheduler->isRunning()) {
        scheduler->execute();
    }
}

std::shared_ptr<Process> SchedulerManager::createUniqueProcess(String name, int id) {
    std::shared_ptr<Process> existingProcess = findProcess(name);

    if (existingProcess != nullptr) {
        return existingProcess;
    } else {
        String pname = generateUniqueProcessName(id);
        Process::RequirementFlags reqFlags = { true, 1, true, GlobalConfig::getInstance()->getMemPerProcess() };

        std::shared_ptr<Process> newProcess = std::make_shared<Process>(id, pname, reqFlags);
        newProcess->generateRandomCommands();

        scheduler->addProcess(newProcess);
        return newProcess;
    }
}

std::unordered_map<String, std::shared_ptr<Process>> SchedulerManager::getAllProcesses() const {
    return scheduler->getProcessMap();
}

std::shared_ptr<Process> SchedulerManager::findProcess(String processName) {
    return scheduler->findProcess(processName);
}

String SchedulerManager::generateUniqueProcessName(int id) {
    std::stringstream ss;
    ss << "P" << id;
    return ss.str();
}

// Constructor
SchedulerManager::SchedulerManager() {
    if (GlobalConfig::getInstance()->getScheduler() == "fcfs") {
        scheduler = std::make_shared<FCFSScheduler>();
    } else if (GlobalConfig::getInstance()->getScheduler() == "rr") {
        scheduler = std::make_shared<RRScheduler>();
    }
}
