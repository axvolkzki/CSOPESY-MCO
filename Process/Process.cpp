#include <ctime>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <random>
#include <chrono> // Include chrono for timestamp
#include <thread>
#include <memory>
#include <vector>

#include "Process.h"
#include "../TypedefRepo.h"
#include "../Console/MainConsole.h"
#include "../Config/GlobalConfig.h"
#include "../Command/ICommand.h"
#include "../Command/PrintCommand.h"


Process::Process(int pid, String name, RequirementFlags requirementFlags)
	: pid(pid), name(name), requirementFlags(requirementFlags), currentState(READY), commandCounter(0)
{
}

void Process::addCommand(ICommand::CommandType commandType)
{

	if (commandType == ICommand::IO) {
		// TODO: Implement I/O command handling

	}
	 else if (commandType == ICommand::PRINT) {
	 	String toPrint = "Hello, World!";
	 	const std::shared_ptr<ICommand> print = std::make_shared<PrintCommand>(this->pid, toPrint);
	 	this->commandList.push_back(print);
	 }
	//else if (commandType == ICommand::PRINT) {
	//	String toPrint = "Random message " + std::to_string(i); // Varying messages
	//	const std::shared_ptr<ICommand> print = std::make_shared<PrintCommand>(this->pid, toPrint);
	//	this->commandList.push_back(print);
	//}
}

// void Process::test_generateRandomCommands(int limit)
void Process::generateRandomCommands()
{
	int limit = GlobalConfig::getInstance()->getRandomInstructionCount();

	for (int i = 0; i < limit; i++) {
		addCommand(ICommand::PRINT);
	}
	


	//int limit = GlobalConfig::getInstance()->getRandomInstructionCount();
	//std::random_device rd;
	//std::mt19937 gen(rd());
	//std::uniform_int_distribution<> dis(0, 1);  // Assuming 0: PRINT, 1: IO (you can extend this)

	//for (int i = 0; i < limit; i++) {
	//	auto commandType = static_cast<ICommand::CommandType>(dis(gen));
	//	addCommand(commandType);
	//}
}

//void Process::updateState()
//{
//	if (this->getRemainingTime() < this->getLinesOfCode()) {
//		this->currentState(RUNNING);
//	}
//	else {
//
//	}
//}

/**
* This is called by CPU core worker threads to execute the current command in the process. 
*/
void Process::executeCurrentCommand() const
{
	if (commandCounter < commandList.size()) {
		commandList[commandCounter]->execute(); // Executes the command
		std::cout << "Executing command at index " << commandCounter << std::endl; // Debug output
	}
	else {
		std::cerr << "No command to execute at index " << commandCounter << std::endl;
	}
}

void Process::moveToNextLine()
{
	std::lock_guard<std::mutex> lock(processMutex);

	if (commandCounter < commandList.size()) {
		this->commandCounter++;
		std::cout << "Moved to next command, counter: " << commandCounter << std::endl; // Debug output
	}
	else {
		std::cout << "All commands executed, cannot move to next line." << std::endl;
	}

}

bool Process::isFinished() const
{
	return this->commandCounter == this->commandList.size();
}

int Process::getRemainingTime() const
{
	return this->commandList.size() - this->commandCounter;
}

int Process::getCommandCounter() const
{
	return this->commandCounter;
}

int Process::getLinesOfCode() const
{
	return this->commandList.size();
}

int Process::getPID() const
{
	return this->pid;
}

int Process::getCPUCoreID() const
{
	return this->cpuCoreID;
}

Process::ProcessState Process::getState() const
{
	return this->currentState;
}

String Process::getName() const
{
	return this->name;
}

void Process::setCPUCoreID(int coreID)
{
	this->cpuCoreID = coreID;
}

void Process::setState(ProcessState state)
{
	this->currentState = state;
}
