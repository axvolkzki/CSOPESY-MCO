#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>  // for std::quoted
#include <algorithm> // for transform() - converting string to lowercase
#include <cstdint>
#include <Windows.h>

#include "MainConsole.h"
#include "../TypedefRepo.h"
#include "../Console/ConsoleManager.h"
#include "../Config/GlobalConfig.h"
#include "../Process/Process.h"
#include "../Event/KeyboardEventHandler.h"
#include "../Scheduler/GlobalScheduler.h"


GlobalConfig globalConfig;


// Constructor: Set the name of the console when MainConsole is instantiated
MainConsole::MainConsole() : AConsole("MainConsole") {}

// Override of onEnabled: This is called when the screen is shown for the first time
void MainConsole::onEnabled() {
	ASCIITextHeader();
}

// Override of display: Called to draw the screen each frame
void MainConsole::display() {
	// ASCIITextHeader();
}

// Override of process: Handle input commands or other processes here
void MainConsole::process() {
	String commandMain;
	bool isFirstCommand = true;
	bool isValidCommand;

	while (true) {
		std::cout << "Enter a command: ";
		std::getline(std::cin, commandMain);

		// Check if the first command is valid
		if (isFirstCommand) {
			isValidCommand = isValidFirstCommand(commandMain);
			
			if (!isValidCommand) {
				std::cerr << "Error: Please initialize the program first. Type command \"initialize\"\n" << std::endl;
				continue; // Re-prompt for the command without displaying the header
			}
			else {
				if (commandMain == "initialize") {
					std::cout << "Initializing the program...\n" << std::endl;
					String filename = "config.txt";

					// Load the config file
					if (globalConfig.loadConfigFile(filename)) {
						std::cout << "Config file loaded successfully.\n" << std::endl;
						globalConfig.printConfig();
						std::cout << "______________________________________________________________\n";
						std::cout << " " << std::endl;

						isFirstCommand = false;
					}
					else {
						std::cerr << "Error: Could not load the config file. Please check the file.\n" << std::endl;
						exit(1);
					}
					
					continue;
				}
				else if (commandMain == "exit") {
					ConsoleManager::getInstance()->exitApplication();
					exit(0);
				}
			}
		}	// If first command end
		
		// Process the commands after the first command or initializing the program
		if (!isFirstCommand) {
			isValidCommand = validateCommand(commandMain);

			if (!isValidCommand) {
				if (commandMain.substr(0, 9) == "screen -s" && commandMain.length() <= 9) {
					std::cerr << "Error: No process name provided. Please try again." << std::endl;
				}
				else if (commandMain.substr(0, 9) == "screen -r" && commandMain.length() <= 9) {
					/*std::cerr << "Error: You are in Main Screen. There is no previous screen. Please try again." << std::endl;*/
					std::cerr << "Error: No process name provided. Please try again." << std::endl;
				}
				else {
					std::cout << commandMain << " command not recognized. Please try again." << std::endl;
				}
				continue; // Re-prompt for the command without displaying the header
			}

			// If the command is recognized
			if (commandMain == "initialize") {
				std::cout << "Program already initialized.\n" << std::endl;
				continue;
			}
			else if (commandMain == "exit") {
				ConsoleManager::getInstance()->exitApplication();
				exit(0);
			}
			else if (commandMain == "clear") {
				system("cls");
				onEnabled();
			}
			else if (commandMain.substr(0, 9) == "screen -s" && commandMain.length() > 9) {			// ensure there is process name
				// Check if the process name is already in the console table
				if (ConsoleManager::getInstance()->isScreenRegistered(commandMain.substr(10))) {
					std::cerr << "Error: Process name " << commandMain.substr(10) << " already exists. Please use another name." << std::endl;
					continue;
				}

				ConsoleManager::getInstance()->exitApplication();									// Stop the main console process
				
				String processName = commandMain.substr(10, commandMain.length() - 10);				// get the process name
				int processID = ++processCounter;												// Increment the process counter
				Process::RequirementFlags processReqFlags = { true, 1, true, 1 };		// Set the requirement flags


				// Create a new process and attach it to a new screen
				//std::shared_ptr<Process> newProcess = std::make_shared<Process>(processID, processName, processReqFlags);	// Create a new process
				//std::shared_ptr<BaseScreen> newScreen = std::make_shared<BaseScreen>(newProcess, processName);	// Create a new screen
				std::shared_ptr<Process> newProcess = GlobalScheduler::getInstance()->createUniqueProcess(processName);
				std::shared_ptr<BaseScreen> newScreen = std::make_shared<BaseScreen>(newProcess, newProcess->getName());
				
				// Register the new screen and switch to it
				ConsoleManager::getInstance()->registerScreen(newScreen);					// Register the new screen
				ConsoleManager::getInstance()->switchToScreen(processName);					// Switch to the new screen
				ConsoleManager::getInstance()->process();									// Process the new screen
				ConsoleManager::getInstance()->drawConsole();								// Draw the new screen
			}
			else if (commandMain.substr(0, 9) == "screen -r") {
				ConsoleManager::getInstance()->exitApplication();					// Stop the main console process
				String processName = commandMain.substr(10);						// Get the process name

				ConsoleManager::getInstance()->switchToScreen(processName);			// Switch to the previous screen
				ConsoleManager::getInstance()->process();							// Process the previous screen
				ConsoleManager::getInstance()->drawConsole();						// Draw the previous screen
			}
			else if (commandMain == "screen -ls") {
				// List all the screens
				/*std::cout << "CPU utilization: " << Scheduler::getInstance()->getCPUUtilization() << "%" << std::endl;*/
				/*std::cout << "Cores used: " << Scheduler::getInstance()->getCoresUsed() << std::endl;*/

				std::cout << "CPU utilization: " << std::endl;
				std::cout << "Cores used: " << std::endl;
				std::cout << "Cores available: " << std::endl;
				std::cout << " " << std::endl;

				std::cout << "______________________________________________________________\n";
				std::cout << "Running processes: \n";
				
				std::cout << " " << std::endl;

				std::cout << "Finished processes: \n";
				std::cout << "______________________________________________________________\n";

			}
			else if (commandMain == "scheduler-test") {
				std::cout << "Testing the scheduler...\n";
				std::cout << "Generating a batch of dummy processes...\n" << std::endl;

				globalConfig.printConfig();

				uint32_t batchProcessFreq = globalConfig.getBatchProcessFreq();	
				/*std::cout << "Batch Process Frequency: " << batchProcessFreq << std::endl;
				std::cout << globalConfig.getBatchProcessFreq() << std::endl;*/
				
				// Get the total number of processes
				uint32_t totalProcesses = ConsoleManager::getInstance()->getTotalScreens();
				// Debugger
				/*std::cout << "Total number of processes: " << totalProcesses << std::endl;*/

				// CPU Cycle Counter
				int cpuCycleCounter = 0;

				KeyboardEventHandler KeyboardEventHandler;

				

				// Continuously generate processes until it was stopped by the command "scheduler-stop" or when a new process is created
				while (isSchedulerStop == false) {
					ConsoleManager::getInstance()->PollKeyboardInput(KeyboardEventHandler);

					if (batchProcessFreq != 0 && cpuCycleCounter % batchProcessFreq == 0) {
						// Process name
						String processName = "Process" + std::to_string(++processCounter);
						// Create a new process
						std::shared_ptr<Process> newProcess = GlobalScheduler::getInstance()->createUniqueProcess(processName);
						std::shared_ptr<BaseScreen> newScreen = std::make_shared<BaseScreen>(newProcess, newProcess->getName());
						ConsoleManager::getInstance()->registerScreen(newScreen);
						//ConsoleManager::getInstance()->switchToScreen(newProcess->getName());
						/*ConsoleManager::getInstance()->process();*/
						//ConsoleManager::getInstance()->drawConsole();
					}
					else if (batchProcessFreq == 0) {
						std::cerr << "Error: batchProcessFreq is zero." << std::endl;
						// Handle the error appropriately
					}
					cpuCycleCounter++;
				}

				// Print the number of processes generated
				std::cout << "Total number of processes generated: " << ConsoleManager::getInstance()->getTotalScreens() << std::endl;

				// Add a small sleep to prevent CPU overuse
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
			else if (commandMain == "scheduler-stop") {
				if (isSchedulerStop) {
					std::cout << "No generating processes.\n" << std::endl;
				}
				else {
					std::cout << "Stopping the scheduler...\n" << std::endl;
					isSchedulerStop = true;
				}
			}
			else if (commandMain == "report-util") {
				std::cout << "Generating CPU utilization report. \n";

				std::cout << "Process Name List: \n";
				std::cout << " " << std::endl;
				ConsoleManager::getInstance()->printScreenNames();
			}
			else {
				recognizeCommand(commandMain);
				continue;
			}
		}	// If not first command end
	}	// Main loop end
}	// Process end

bool MainConsole::getIsSchedulerStop()
{
	return isSchedulerStop;
}


void MainConsole::ASCIITextHeader() const {
	std::cout << "  ____    ____      ___     ____    _______    ____    __   __		\n";
	std::cout << " / ___|  / ___|    / _ \\   |  _ \\   |  ___|   / ___|   \\ \\ / /	\n";
	std::cout << "| |      \\___ \\   | | | |  | |_) |  |  __|    \\___ \\    \\ V /	\n";
	std::cout << "| |___    ___) |  | |_| |  |  __/   | |___     ___) |    | |			\n";
	std::cout << " \\____|  |____/    \\___/   |_|      |_____|   |____/     |_|		\n";

	std::cout << "______________________________________________________________\n";

	HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_color, 10);
	std::cout << "Welcome to CSOPESY Emulator!\n";
	std::cout << "\n";

	displayDevelopers();
	std::cout << "______________________________________________________________\n";

	SetConsoleTextAttribute(console_color, 14);
	std::cout << "Type 'exit' to quit, 'clear' to clear the screen\n";
	SetConsoleTextAttribute(console_color, 15);
}


void MainConsole::displayDevelopers() const {
	HANDLE console_color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console_color, 15);

	std::cout << "Developers: \n";
	std::cout << "1. Abenoja, Amelia Joyce L. \n";
	std::cout << "2. Cuales, Bianca Mari A. \n";
	std::cout << "3. Culala, Mary Erika L. \n";
	std::cout << "4. Uy,Gleezell Vina A. \n";
	std::cout << "\n";
	std::cout << "Last Updated: 10-24-2024\n";
}

bool MainConsole::isValidFirstCommand(String command) const {
	bool isValid = false;

	String commandList[] = { "initialize", "exit" };

	// Check if the first word of the input is a valid first command
	String inputCommand = command.substr(0, command.find(" "));
	std::transform(inputCommand.begin(), inputCommand.end(), inputCommand.begin(), ::tolower);

	for (String command : commandList) {
		if (inputCommand == command) {
			isValid = true;
			break;
		}
	}

	return isValid;
}


bool MainConsole::validateCommand(String& input) const {
	bool isValid = false;

	String commandList[] = { "initialize", "exit", "clear",
							"scheduler-test", "scheduler-stop", "report-util",
							"screen" };

	// Check if the first word of the input is a valid command
	String inputCommand = input.substr(0, input.find(" "));
	std::transform(inputCommand.begin(), inputCommand.end(), inputCommand.begin(), ::tolower);

	for (String command : commandList) {
		if (inputCommand == command) {
			if (command == "screen") {
				if (isValidScreenCommand(input)) {
					isValid = true;
					break;
				}
			}
			else {
				isValid = true;
				break;
			}
		}
	}


	return isValid;
}

bool MainConsole::isValidScreenCommand(String command) const {
	bool isValid = false;

	String screenCommandList[] = { "screen -s", "screen -ls", "screen -r" };

	for (String screenCommand : screenCommandList) {
		if (command.substr(0, 9) == screenCommand) {
			// Check if the command has process name after the screen command
			if (command.length() > 9) {
				isValid = true;
				break;
			}
		}
		else if (command.substr(0, 10) == screenCommand) {
			isValid = true;
			break;
		}
	}

	return isValid;
}


void MainConsole::recognizeCommand(String command) const {
	std::cout << command << " command recognized. Doing something...\n";
}
