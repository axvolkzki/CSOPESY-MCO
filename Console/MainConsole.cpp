#include <iostream>
#include <algorithm> // for transform() - converting string to lowercase
#include <cstdint>
#include <Windows.h>

#include "MainConsole.h"
#include "../TypedefRepo.h"
#include "../Console/ConsoleManager.h"
#include "../Config/GlobalConfig.h"
#include "../Process/Process.h"
#include "../Scheduler/SchedulerManager.h"
#include <conio.h>


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

bool MainConsole::getIsSchedulerStop() const
{
	return false;
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
					
					GlobalConfig::getInstance()->printConfig();	// Load the configuration file
					std::cout << " " << std::endl;
					
					isFirstCommand = false;
					
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

				if (ConsoleManager::getInstance()->isScreenRegistered(commandMain.substr(10))) {
					std::cerr << "Error: Process name " << commandMain.substr(10) << " already exists. Please use another name." << std::endl;
					continue;
				}

				ConsoleManager::getInstance()->exitApplication();									// Stop the main console process
				
				String processName = commandMain.substr(10, commandMain.length() - 10);				// get the process name
				processCounter = ConsoleManager::getInstance()->getTotalScreens();
				int processID = ++processCounter;													// Increment the process counter
				int quantum = GlobalConfig::getInstance()->getQuantumCycles();						// Get the quantum cycles
				Process::RequirementFlags processReqFlags = { true, 1, true, 1 };					// Set the requirement flags

				std::shared_ptr<Process> newProcess = SchedulerManager::getInstance()->createUniqueProcess(processName, processID);
				std::shared_ptr<BaseScreen> newScreen = std::make_shared<BaseScreen>(newProcess, newProcess->getName());

				// Register the new screen and switch to it
				ConsoleManager::getInstance()->registerScreen(newScreen);					// Register the new screen
				ConsoleManager::getInstance()->switchToScreen(processName);					// Switch to the new screen
				
				// Process and draw the new screen
				ConsoleManager::getInstance()->process();									// Process the new screen
				ConsoleManager::getInstance()->drawConsole();								// Draw the new screen
			}
			else if (commandMain.substr(0, 9) == "screen -r") {
				ConsoleManager::getInstance()->exitApplication();							// Stop the main console process
				String processName = commandMain.substr(10);								// Get the process name

				ConsoleManager::getInstance()->switchToScreen(processName);					// Switch to the previous screen
				ConsoleManager::getInstance()->process();									// Process the previous screen
				ConsoleManager::getInstance()->drawConsole();								// Draw the previous screen
			}
			else if (commandMain == "screen -ls") {

				std::cout << "CPU utilization: " << std::endl;			// TODO: Missing implementation
				std::cout << "Cores used: " << std::endl;				// TODO: Missing implementation
				std::cout << "Cores available: " << std::endl;			// TODO: Missing implementation
				std::cout << " " << std::endl;

				std::cout << "______________________________________________________________\n";
				// Print running processes
				std::cout << "Running Processes:\n";
				

				// Print finished processes
				std::cout << "\nFinished Processes:\n";
				

				std::cout << "______________________________________________________________\n";

			}
			else if (commandMain == "scheduler-test") {
				std::cout << "Running scheduler test...\n";
				
				int batchProcessFreq = GlobalConfig::getInstance()->getBatchProcessFreq();
				int cpuCycleCounter = 0;
				this->processCounter = ConsoleManager::getInstance()->getTotalScreens();
				
				std::cout << "Enter command: ";
				while (this->isSchedulerStop == false) {
					
					if (_kbhit()) {
						std::string input;
						std::getline(std::cin, input);
						if (input == "scheduler-stop") {
							std::cout << "Stopping the scheduler...\n";
							isSchedulerStop = true;
							break;
						}
					}


					if (cpuCycleCounter % batchProcessFreq == 0) {
						int processID = ++processCounter;
						String processName = "Process" + std::to_string(processID);
						std::shared_ptr<Process> newProcess = SchedulerManager::getInstance()->createUniqueProcess(processName, processID);
						std::shared_ptr<BaseScreen> newScreen = std::make_shared<BaseScreen>(newProcess, newProcess->getName());
						ConsoleManager::getInstance()->registerScreen(newScreen);
					}
					cpuCycleCounter++;
					SchedulerManager::getInstance()->tick();
				}
			}
			else if (commandMain == "scheduler-stop") {
				std::cout << "Stopping the scheduler...\n";
				/*SchedulerManager::getInstance()->stopScheduler();*/
				isSchedulerStop = true;
			}
			else if (commandMain == "report-util") {
				std::cout << "Generating CPU utilization report... exporting to file...\n";
			}
			else {
				recognizeCommand(commandMain);
				continue;
			}
		}	// If not first command end
	}	// Main loop end
}	// Process end




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
	std::cout << "\n";
	std::cout << "Last Updated: 11-11-2024\n";
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
			if (command.length() > 9) {			// Check if the command has process name after the screen command
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
