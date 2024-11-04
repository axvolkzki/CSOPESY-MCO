#pragma once
#include <sstream>
#include <unordered_map>

#include "../Threading/IETThread.h"
#include "../Process/Process.h"
#include "../TypedefRepo.h"

static const String FCFS_SCHEDULER_NAME = "FCFSScheduler";
static const String ROUND_ROBIN_SCHEDULER_NAME = "RoundRobinScheduler";

class AScheduler : public IETThread
{
public:
    enum SchedulingAlgorithm {
        FCFS,
        ROUND_ROBIN
    };

    AScheduler(SchedulingAlgorithm schedulingAlgo, int pid, String processName);

	void addProcess(std::shared_ptr<Process> process);
	std::shared_ptr<Process> findProcess(String processName);
	void run() override;
    void stop();

    virtual void init() = 0;
    virtual void execute() = 0;

	struct ProcessInfo {
		int pid;
		String name;
        int cpuID;
        int lineCounter;
        int lineOfCode;
        int remainingTime;
	};

protected:
	SchedulingAlgorithm schedulingAlgo;
	int pid;
	String processName;
	std::unordered_map<String, std::shared_ptr<Process>> processMap;
	bool running = true;
};

