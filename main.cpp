#include "Scheduler/GlobalScheduler.h"
#include "Console/ConsoleManager.h"
#include "Scheduler/ResourceEmulator.h"
#include "Threading/SchedulerWorker.h"


using namespace std;

int main() {
	GlobalScheduler::initialize();													// Initialize the instance of GlobalSize
	ConsoleManager::initialize();	// Initialize the instance of ConsoleManager	
	ResourceEmulator::initialize();													// Initialize the instance of ResourceEmulator


	// Thread for the CPU Core Worker
	// std::thread(&SchedulerWorker::run).detach();									// detach thread for indpendent execution. without this, join() function must be called
	
	SchedulerWorker schedulerWorker;												// Create SchedulerWorker instance
	std::thread workerThread(&SchedulerWorker::run, &schedulerWorker);				// Start the worker thread
	workerThread.detach();

	ResourceEmulator::getInstance()->startAllCPUs;									// Initialize the instance of ResourceEmulato
	//GlobalScheduler::getInstance()->test_randomCreateProcesses(50);				// Create random processes for testing
	bool running = true;

	// Main loop
	while (running) {

		ConsoleManager::getInstance()->process();									// contains handling of logic and other non-drawing operations; call its instance
		ConsoleManager::getInstance()->drawConsole();								// refreshes the screen with the updated information
		
		// Check if still running
		running = ConsoleManager::getInstance()->isRunning();

		// Optional delay to reduce CPU usage
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}


	// Clean up by destroying the instance of ConsoleManager
	ResourceEmulator::destroy();
	ConsoleManager::destroy();
	GlobalScheduler::destroy();

	return 0;
}