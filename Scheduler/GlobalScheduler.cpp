#include "GlobalScheduler.h"
#include "../Config/GlobalConfig.h"

GlobalScheduler* GlobalScheduler::sharedInstance = nullptr;

GlobalScheduler::GlobalScheduler() : pidCounter(0), currentAlgo(AScheduler::FCFS) {
	// Initialize with default FCFS scheduler
	createScheduler(AScheduler::FCFS);
}

void GlobalScheduler::createScheduler(AScheduler::SchedulingAlgorithm algo) {
	GlobalConfig config;

	if (config.getScheduler() == "rr") {
		currentAlgo = AScheduler::ROUND_ROBIN;
	}
	else {
		currentAlgo = algo;
	}
	

	// Create the appropriate scheduler type based on the algorithm
	switch (currentAlgo) {
	case AScheduler::FCFS:
		scheduler = std::make_shared<FCFSScheduler>(pidCounter,
			FCFS_SCHEDULER_NAME);
		break;

	case AScheduler::ROUND_ROBIN:
		scheduler = std::make_shared<RoundRobinScheduler>(pidCounter,
			ROUND_ROBIN_SCHEDULER_NAME, config.getQuantumCycles());
		break;

	default:
		throw std::runtime_error("Unsupported scheduling algorithm");
	}
}

void GlobalScheduler::initialize(AScheduler::SchedulingAlgorithm algo) {
	if (sharedInstance == nullptr) {
		sharedInstance = new GlobalScheduler();
		/*sharedInstance->createScheduler(algo);*/
	}
}

void GlobalScheduler::destroy()
{
	delete sharedInstance;
	sharedInstance = nullptr;
}

void GlobalScheduler::tick()
{
	this->scheduler->execute();
}

/**
* \brief Creates a new process (if none exists). If a process with the same name exists, it returns existing process.
* \param processName Name of the process to create.
* \return Pointer to the process.
*/
std::shared_ptr<Process> GlobalScheduler::createUniqueProcess(String name) 
{
	std::shared_ptr<Process> existingProcess = this->findProcess(name);

	if (existingProcess != nullptr) {
		return existingProcess;
	}
	else {
		Process::RequirementFlags reqFlags = { true, 1, true, 1 };

		if (name == "") {
			name = this->generateProcessName();
		}
		std::shared_ptr<Process> newProcess = std::make_shared<Process>(this->pidCounter, name, reqFlags);
		newProcess->test_generateRandomCommands(10);

		// put new process to ready queue
		this->scheduler->addProcess(newProcess);
		this->pidCounter++;

		return newProcess;
	}

}

std::shared_ptr<Process> GlobalScheduler::findProcess(String processName)
{
	return this->scheduler->findProcess(processName);
}

String GlobalScheduler::generateProcessName()
{
	std::stringstream ss;
	ss << "Process" << this->pidCounter;
	return ss.str();
}