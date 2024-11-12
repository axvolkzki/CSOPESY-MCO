#pragma once
#include "../TypedefRepo.h"
#include "AScheduler.h"
#include "FCFSScheduler.h"
#include "RRScheduler.h"
#include <memory>
#include <mutex>

class SchedulerManager {
public:
    static SchedulerManager* getInstance();  
    static void initialize();  
    static void destroy();  

    void tick();  

    std::shared_ptr<Process> createUniqueProcess(String name, int id);  
    std::unordered_map<String, std::shared_ptr<Process>> getAllProcesses() const;

private:
    SchedulerManager();  
    ~SchedulerManager() = default;

    SchedulerManager(SchedulerManager const&) {};  
    SchedulerManager& operator=(SchedulerManager const&) { return *this; };  
    static SchedulerManager* sharedInstance;  

    std::shared_ptr<AScheduler> scheduler;  
    std::mutex schedulerMutex;  
    std::shared_ptr<Process> findProcess(String processName);
    String generateUniqueProcessName(int id);
};
