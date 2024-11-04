#pragma once
#include <memory>
#include "AScheduler.h"
#include "FCFSScheduler.h" // You'll need to create this
#include "RoundRobinScheduler.h" // You'll need to create this

class GlobalScheduler {
public:
    static GlobalScheduler* getInstance() {
        if (sharedInstance == nullptr) {
            sharedInstance = new GlobalScheduler();
        }
        return sharedInstance;
    }

    static void initialize(AScheduler::SchedulingAlgorithm algo = AScheduler::FCFS);
    static void destroy();
    void tick();

    std::shared_ptr<Process> createUniqueProcess(String name);
    AScheduler::SchedulingAlgorithm getCurrentAlgorithm() const { return currentAlgo; }

private:
    GlobalScheduler();
    std::shared_ptr<AScheduler> scheduler;
    static GlobalScheduler* sharedInstance;
    int pidCounter;
    AScheduler::SchedulingAlgorithm currentAlgo;

    std::shared_ptr<Process> findProcess(String processName);
    String generateProcessName();
    void createScheduler(AScheduler::SchedulingAlgorithm algo);
};