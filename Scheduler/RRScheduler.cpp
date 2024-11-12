#include "RRScheduler.h"
#include "../Config/GlobalConfig.h"
#include "../Memory/FlatMemoryAllocator.h"

void RRScheduler::init() {
    timeSlice = GlobalConfig::getInstance()->getQuantumCycles();
    processQueue = std::queue<std::shared_ptr<Process>>();
    currentProcess = nullptr;

    processMap = std::unordered_map<String, std::shared_ptr<Process>>();
}

void RRScheduler::execute() {
    if (currentProcess != nullptr) {
        currentProcess->updateState(Process::RUNNING);
        currentProcess->executeCurrentCommand();
        currentProcess->moveToNextLine();

        if (currentProcess->isFinished()) {
            currentProcess->updateState(Process::FINISHED);
            FlatMemoryAllocator::getInstance()->deallocate(currentProcess);  // Release memory
            currentProcess = nullptr; 
        } else {
            processQueue.push(currentProcess); 
        }
    }

    if (currentProcess == nullptr && !processQueue.empty()) {
        currentProcess = processQueue.front();
        processQueue.pop();

        void* allocatedMemory = FlatMemoryAllocator::getInstance()->allocate(GlobalConfig::getInstance()->getMemPerProcess());
        if (allocatedMemory == nullptr) {  
            processQueue.push(currentProcess);  
            currentProcess = nullptr;  
        } else {
            quantumCounter++;
            if (quantumCounter == timeSlice) {
                FlatMemoryAllocator::getInstance()->saveSnapshot(quantumCounter);
                quantumCounter = 0;
            }
        }
    }
}

void RRScheduler::addProcess(std::shared_ptr<Process> process) {
    processQueue.push(process);
    processMap[process->getName()] = process;
}
