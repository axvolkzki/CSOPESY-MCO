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
	
	// Start SchedulerWorker for this screen's process
    SchedulerWorker schedulerWorker;
    schedulerWorker.update(true);
    std::thread schedulerThread(&SchedulerWorker::run, &schedulerWorker);
    schedulerThread.detach();  // Detach thread to run independently

	// Command input loop for user
    while (true) {
        std::cout << "\nEnter a command: ";
        std::getline(std::cin, commandBaseScreen);

        if (commandBaseScreen == "exit") {
            schedulerWorker.update(false); // Stop the scheduler worker
            ConsoleManager::getInstance()->switchConsole(MAIN_CONSOLE);
            break;
        } else if (commandBaseScreen == "process-smi") {
            display();  // Display process information
        } else {
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