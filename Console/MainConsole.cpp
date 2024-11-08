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
#include "../Threading/SchedulerWorker.h"

std::atomic<bool> isSchedulerStop{false};  // Atomic flag to control stopping the scheduler
SchedulerWorker schedulerWorker;           // Instance of SchedulerWorker
std::thread schedulerThread;               // Thread to run the scheduler worker


// GlobalConfig globalConfig;
GlobalConfig& config = GlobalConfig::getInstance();


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
					
					config.printConfig();
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
				int processID = ++processCounter;													// Increment the process counter
				Process::RequirementFlags processReqFlags = { true, 1, true, 1 };					// Set the requirement flags

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
			/*
			else if (commandMain == "scheduler-test") {
				isSchedulerStop = false;            // Reset stop flag

				schedulerWorker.update(true);       // Tell the worker to start processing
				schedulerThread = std::thread(&SchedulerWorker::run, &schedulerWorker);  // Launch in a new thread

				std::cout << "Scheduler started and is running in the background.\n";
				std::cout << "Testing the scheduler...\n";
				std::cout << "Generating a batch of processes...\n" << std::endl;

				config.printConfig();
				std::cout << " " << std::endl;

				uint32_t batchProcessFreq = config.getBatchProcessFreq();	
				std::cout << "Batch Process Frequency: " << batchProcessFreq << std::endl;

				// CPU Cycle Counter
				int cpuCycleCounter = 0;

				KeyboardEventHandler keyboardEventHandler;

				// Continuously generate processes until it was stopped by the command "scheduler-stop" 
				while (!keyboardEventHandler.getIsSchedulerStop()) {
					// Poll keyboard input for interrupts
					ConsoleManager::getInstance()->PollKeyboardInput(keyboardEventHandler);

					// If paused, wait for user input
					if (keyboardEventHandler.isPaused()) {
						std::cout << "Scheduler waiting for user input..." << std::endl;
						while (keyboardEventHandler.isPaused() && !keyboardEventHandler.getIsSchedulerStop()) {
							ConsoleManager::getInstance()->PollKeyboardInput(keyboardEventHandler);
							std::this_thread::sleep_for(std::chrono::milliseconds(100));
						}
						keyboardEventHandler.resumeScheduler();  // Resume once input is processed
					}

					// Process generation
					if (batchProcessFreq != 0 && cpuCycleCounter % batchProcessFreq == 0) {
						String processName = "Process" + std::to_string(++processCounter);
						try {
							std::shared_ptr<Process> newProcess = GlobalScheduler::getInstance()->createUniqueProcess(processName);
							std::shared_ptr<BaseScreen> newScreen = std::make_shared<BaseScreen>(newProcess, newProcess->getName());
							ConsoleManager::getInstance()->registerScreen(newScreen);
						} catch (const std::exception& e) {
							std::cerr << "Error creating process: " << e.what() << std::endl;
						}
					} else if (batchProcessFreq == 0) {
						std::cerr << "Error: batchProcessFreq is zero." << std::endl;
						break;
					}

					cpuCycleCounter++;
					std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Prevent CPU overuse
				}

				// Print the number of processes generated
				std::cout << "Total number of processes generated: " << ConsoleManager::getInstance()->getTotalScreens() << std::endl;
			}
			*/
			else if (commandMain == "scheduler-test") {
				if (schedulerThread.joinable()) {
					std::cout << "Scheduler is already running.\n";
					return;
				}

				isSchedulerStop = false;            // Reset stop flag
				schedulerWorker.update(true);       // Tell the worker to start processing
				schedulerThread = std::thread(&SchedulerWorker::run, &schedulerWorker);  // Launch in a new thread

				std::cout << "Scheduler started and is running in the background.\n";

				// Process creation loop in the background
				uint32_t batchProcessFreq = config.getBatchProcessFreq();
				int cpuCycleCounter = 0;

				while (!isSchedulerStop) {
					if (batchProcessFreq != 0 && cpuCycleCounter % batchProcessFreq == 0) {
						// Generate a new process
						String processName = "Process" + std::to_string(++processCounter);

						try {
							std::shared_ptr<Process> newProcess = GlobalScheduler::getInstance()->createUniqueProcess(processName);
							std::shared_ptr<BaseScreen> newScreen = std::make_shared<BaseScreen>(newProcess, newProcess->getName());
							ConsoleManager::getInstance()->registerScreen(newScreen);
							
							// std::cout << "Created new process: " << processName << std::endl;
						} catch (const std::exception& e) {
							std::cerr << "Error creating process: " << e.what() << std::endl;
						}
					}

					cpuCycleCounter++;
					std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Frequency of process generation
				}

				// Stop the scheduler worker if the loop is exited
				if (schedulerThread.joinable()) {
					schedulerWorker.update(false);  // Tell worker to stop
					schedulerThread.join();
				}
				
				std::cout << "Scheduler has stopped generating processes.\n";
			}
			else if (commandMain == "scheduler-stop") {
				if (isSchedulerStop) {
					std::cout << "Scheduler is already stopped.\n";
				} else {
					isSchedulerStop = true;            // Set the stop flag
					schedulerWorker.update(false);     // Tell the worker to stop

					if (schedulerThread.joinable()) {
						schedulerThread.join();        // Wait for the thread to finish
					}

					std::cout << "Scheduler has been stopped.\n";
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
