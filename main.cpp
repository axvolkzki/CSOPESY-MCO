#include "Config/GlobalConfig.h"
#include "Scheduler/SchedulerManager.h"
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
	// 1. Initializers
	//InputManager::initialize();														// Initialize the instance of InputManager
	//FileSystem::initialize();														// Initialize the instance of FileSystem

	//FileSystem::getInstance()->loadConfig();										// Load the configuration file
	//FileSystem::getInstance()->test_createRandomFiles(1000);						// Create random files for testing
	//FileSystem::getInstance()->saveFileSystem();									// Save the file system

	//FileSystem::getInstance()->loadFileSystem();									// Load the file system
	GlobalConfig::initialize();														// Initialize the instance of GlobalConfig
	String configPath = "config.txt";												// Set the path to the configuration file
	GlobalConfig::getInstance()->loadConfigFile(configPath);						// Load the configuration file

	SchedulerManager::initialize();													// Initialize the instance of SchedulerManager
	ConsoleManager::initialize();													// Initialize the instance of ConsoleManager	
	//MessageBuffer::initialize();													// Initialize the instance of MessageBuffer
	ResourceEmulator::initialize();													// Initialize the instance of ResourceEmulator
	//MemoryManager::initialize();													// Initialize the instance of MemoryManager

	// 2. Start all CPU cores (SchedulerWorker threads) in ResourceEmulator
	ResourceEmulator::getInstance()->startAllCPUs();									// Initialize the instance of ResourceEmulato	
	
	// 3. Main loop
	bool running = true;
	while (running) {

		ConsoleManager::getInstance()->process();									// contains handling of logic and other non-drawing operations; call its instance
		ConsoleManager::getInstance()->drawConsole();								// refreshes the screen with the updated information
		running = ConsoleManager::getInstance()->isRunning();						// check if the application is still running	

		// Optional delay to reduce CPU usage
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}


	// 4. Clean up by destroying and resource deallocation
	ResourceEmulator::destroy();
	ConsoleManager::destroy();
	SchedulerManager::destroy();
	GlobalConfig::destroy();

	return 0;
}