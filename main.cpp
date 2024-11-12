#include "Config/GlobalConfig.h"
#include "Console/ConsoleManager.h"

using namespace std;

// columns x rows


int main() {

	GlobalConfig::initialize();														// Initialize the instance of GlobalConfig
	String configPath = "config.txt";												// Set the path to the configuration file
	GlobalConfig::getInstance()->loadConfigFile(configPath);						// Load the configuration file
	ConsoleManager::initialize();													// Initialize the instance of ConsoleManager	

	
	// 3. Main loop
	bool running = true;
	while (running) {

		ConsoleManager::getInstance()->process();									// contains handling of logic and other non-drawing operations; call its instance
		ConsoleManager::getInstance()->drawConsole();								// refreshes the screen with the updated information
		running = ConsoleManager::getInstance()->isRunning();						// check if the application is still running	

		// Optional delay to reduce CPU usage
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}


	// Clean up by destroying and resource deallocation
	ConsoleManager::destroy();
	GlobalConfig::destroy();

	return 0;
}