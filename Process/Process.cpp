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
	// Get the instance of GlobalConfig
	GlobalConfig& config = GlobalConfig::getInstance();

	// max and minimum
	int min = config.getMinIns();
	int max = config.getMaxIns();

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);

	int limit = dis(gen);

	for (int i = 0; i < limit; i++) {
		addCommand(ICommand::PRINT);
	}
	


	// for (int i = 0; i < limit; i++)
	// {
	// 	auto commandType = static_cast<ICommand::CommandType>(dis(gen));

	// 	if (commandType == ICommand::IO) {
	// 		// TODO: Implement I/O command handling

	// 	}
	// 	else if (commandType == ICommand::PRINT) {
	// 		String toPrint = "Hello, World!";
	// 		const std::shared_ptr<ICommand> print = std::make_shared<PrintCommand>(this->pid, toPrint);
	// 		this->commandList.push_back(print);
	// 	}

	// 	// String toPrint = "This is a sample print.";
	// 	// const std::shared_ptr<ICommand> print = std::make_shared<PrintCommand>(this->pid, toPrint);
	// 	// this->commandList.push_back(print);
	// }
}

void Process::executeCurrentCommand() const
{
	commandList[commandCounter]->execute(); // Executes the command
    std::cout << "Executing command at index " << commandCounter << std::endl; // Debug output
	// this->commandList[this->commandCounter]->execute();
	// if (commandCounter < commandList.size()) {
    //     commandList[commandCounter]->execute();
    // } else {
    //     std::cerr << "No command to execute at index " << commandCounter << std::endl;
    // }
}

void Process::moveToNextLine()
{
	// if (commandCounter < commandList.size()) {
    //     commandCounter++;
    // }

	this->commandCounter++;
	std::cout << "Moved to next command, counter: " << commandCounter << std::endl; // Debug output

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