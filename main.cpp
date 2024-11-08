#include "Config/GlobalConfig.h"
#include "Scheduler/GlobalScheduler.h"
#include "Console/ConsoleManager.h"
#include "Scheduler/ResourceEmulator.h"
#include "Threading/SchedulerWorker.h"

using namespace std;

// columns x rows
// void SetConsoleWindowSize(int width, int height) {
// 	const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
// 	const COORD newSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
// 	SetConsoleScreenBufferSize(hConsole, newSize);

// 	const SMALL_RECT rect = { 0, 0, newSize.X - 1, newSize.Y - 1 };
// 	SetConsoleWindowInfo(hConsole, TRUE, &rect);
// }

int main() {
	/*
	InputManager::initialize();														// Initialize the instance of InputManager
	FileSystem::initialize();														// Initialize the instance of FileSystem

	FileSystem::getInstance()->loadConfig();										// Load the configuration file
	FileSystem::getInstance()->test_createRandomFiles(1000);						// Create random files for testing
	FileSystem::getInstance()->saveFileSystem();									// Save the file system

	FileSystem::getInstance()->loadFileSystem();									// Load the file system
	*/

	GlobalConfig& config = GlobalConfig::getInstance(); // Create an instance of GlobalConfig
    config.initialize(); // Call the initialize method on the instance


	GlobalScheduler::initialize();													// Initialize the instance of GlobalSize
	ConsoleManager::initialize();	// Initialize the instance of ConsoleManager	
	
	//MessageBuffer::initialize();													// Initialize the instance of MessageBuffer

	ResourceEmulator::initialize();													// Initialize the instance of ResourceEmulator

	//MemoryManager::initialize();													// Initialize the instance of MemoryManager

	

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