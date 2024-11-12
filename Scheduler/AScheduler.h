#pragma once
#include <memory>
#include <sstream>
#include <unordered_map>

#include "../Threading/IETThread.h"
#include "../Process/Process.h"
#include "../Config/GlobalConfig.h"
#include "../TypedefRepo.h"
#include <queue>

//
static const String FCFS_SCHEDULER_NAME = "FCFSScheduler"; // FCFS scheduler name
static const String RR_SCHEDULER_NAME = "RRScheduler";     // RR scheduler name
//

class AScheduler : public IETThread
{
public:
    enum SchedulingAlgorithm
    { 
        fcfs, 
        rr 
    };  // Enum for different scheduling algorithms

    AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, String processName);

	void addProcess(std::shared_ptr<Process> process);          // Adds process in the readyQueue
    std::shared_ptr<Process> findProcess(String processName);
	void run() override;                                        // Runs the scheduler to handle the readyQueue; 
    void stop();                                                // Stops the scheduler

    virtual void init() = 0;                                    // Initializes the scheduler (virtual)
    virtual void execute() = 0;                                 // Executes the scheduler logic (virtual)

    struct ProcessInfo
    {
        int pid;
        String name;
        int cpuID;
        int lineCounter;
        int linesOfCode;
        int remainingTime;
    };

protected:
    SchedulingAlgorithm algorithm;
    std::vector<Process> processes;
    bool isRunning;
};

