#include <iostream>
#include <format>

#include "../Screen/BaseScreen.h"
#include "../TypedefRepo.h"
#include "../Process/Process.h"
#include "../Console/ConsoleManager.h"



BaseScreen::BaseScreen(std::shared_ptr<Process> process, String processName) : AConsole(processName), attachedProcess(process) {};

void BaseScreen::onEnabled() {
	displayProcessInfo();
}

void BaseScreen::display() {
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
    
	while (true) {
		std::cout << "\nEnter a command: ";
		std::getline(std::cin, commandBaseScreen);
		std::cout << "\n";

		// Increment the current line of code for every tick

		if (commandBaseScreen == "exit") {
			ConsoleManager::getInstance()->exitApplication();			// Exit the application
			ConsoleManager::getInstance()->switchConsole(MAIN_CONSOLE);  // Exit to the previous console
			break;
		}
		else if (commandBaseScreen == "process-smi") {
			if (!attachedProcess->isFinished()) {
				attachedProcess->executeCurrentCommand();
				attachedProcess->moveToNextLine();
			}
			
			display();

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

	/*std::cout << "Process State: " << attachedProcess->getState() << std::endl;
	std::cout << "Command Counter: " << attachedProcess->getCommandCounter() << std::endl;
	std::cout << "Lines of Code: " << attachedProcess->getLinesOfCode() << std::endl;
	std::cout << "CPU Core ID: " << attachedProcess->getCPUCoreID() << std::endl;*/
}