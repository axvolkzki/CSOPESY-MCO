#include <iostream>
#include <format>

#include "../Screen/BaseScreen.h"
#include "../TypedefRepo.h"
#include "../Process/Process.h"
#include "../Console/ConsoleManager.h"
#include "../Threading/SchedulerWorker.h"



BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) : AConsole(processName), attachedProcess(process) {};

void BaseScreen::onEnabled() {
	displayProcessInfo();
}

void BaseScreen::display()
{
	refreshed = attachedProcess->isFinished();

	if (!refreshed) {
		std::cout << "Process Name: " << attachedProcess->getName() << std::endl;
		std::cout << "ID: " << attachedProcess->getPID() << std::endl;
		std::cout << " " << std::endl;
		std::cout << "Current instruction line: " << attachedProcess->getCommandCounter() << std::endl;
		std::cout << "Lines of code: " << attachedProcess->getLinesOfCode() << std::endl;
	}
	else {
		std::cout << "Process Name: " << attachedProcess->getName() << std::endl;
		std::cout << "ID: " << attachedProcess->getPID() << std::endl;
		std::cout << " " << std::endl;
		std::cout << "Finished!" << std::endl;
	}
}


void BaseScreen::process() {
    String commandBaseScreen;

    // Command input loop for user with periodic refresh for updates
    bool running = true;
    while (running) {
        // Periodically refresh display for process updates
        std::this_thread::sleep_for(std::chrono::seconds(2));  // Adjust refresh rate as needed

        std::cout << "\nEnter a command: ";
        std::getline(std::cin, commandBaseScreen);

        if (commandBaseScreen == "exit") {
            ConsoleManager::getInstance()->switchConsole(MAIN_CONSOLE);
            running = false;                // Stop the loop
        }
        else if (commandBaseScreen == "process-smi") {
            std::cout << "______________________________________________________________\n";
            display();  // Display current process information on demand
            std::cout << "______________________________________________________________\n";
        }
        else {
            std::cout << "Command not recognized. Please try again." << std::endl;
        }
    }
}



void BaseScreen::displayProcessInfo() {
	// Display the process information
	std::cout << "Process Name: " << attachedProcess->getName() << std::endl;
	std::cout << "ID: " << attachedProcess->getPID() << std::endl;
	std::cout << " " << std::endl;
	std::cout << "Current instruction line: " << attachedProcess->getCommandCounter() << std::endl;
	std::cout << "Lines of code: " << attachedProcess->getLinesOfCode() << std::endl;
	std::cout << "CPU Core ID: " << attachedProcess->getCPUCoreID() << std::endl;
	std::cout << "Process State: " << attachedProcess->getState() << std::endl;
	std::cout << " " << std::endl;
	std::cout << "______________________________________________________________\n";
}