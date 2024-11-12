#pragma once
#include "AScheduler.h"
#include "../TypedefRepo.h"
#include <queue>
#include "../Memory/FlatMemoryAllocator.h"

class RRScheduler : public AScheduler {
public:
    RRScheduler(int timeSlice = 100) : AScheduler(rr, 0, ""), timeSlice(timeSlice) {}

    void init() override;         
    void execute() override;      

    void addProcess(std::shared_ptr<Process> process);
    std::unordered_map<String, std::shared_ptr<Process>> getProcessMap() const { return processMap; }

private:
    int timeSlice;  
    std::queue<std::shared_ptr<Process>> processQueue; 
    std::shared_ptr<Process> currentProcess;  

    std::unordered_map<String, std::shared_ptr<Process>> processMap;
    int quantumCounter = 0;  // Track quantum cycles
};
