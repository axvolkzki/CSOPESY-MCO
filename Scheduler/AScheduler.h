#pragma once
#include <memory>
#include <sstream>
#include <unordered_map>

#include "../Threading/IETThread.h"
#include "../Process/Process.h"
#include "../TypedefRepo.h"

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

    void addProcess(std::shared_ptr<Process> process); // Adds process
    std::shared_ptr<Process> findProcess(String processName);
    void run() override;
    void stop(); // Stops the scheduler

	// get process map
	std::unordered_map<String, std::shared_ptr<Process>> getProcessMap() const { return processMap; }

    // Common virtual methods to be implemented by derived classes
    virtual void init() = 0;     // Initializes the scheduler (virtual)
    virtual void execute() = 0;  // Executes the scheduler logic (virtual)

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
    SchedulingAlgorithm algorithm;       // Scheduling algorithm type (e.g., FCFS, RR)
	ProcessInfo currentProcessInfo;      // Current process info
	std::unordered_map<String, std::shared_ptr<Process>> processMap; // Map to store processes
	bool isRunning = true;              // Flag to indicate if the scheduler is running
};

