#include <ctime>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <random>
#include <chrono> // Include chrono for timestamp
#include <thread>

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
		// Do nothing

	}
	else if (commandType == ICommand::PRINT) {
		String toPrint = "Hello, World!";
		const std::shared_ptr<ICommand> print = std::make_shared<PrintCommand>(this->pid, toPrint);
		this->commandList.push_back(print);
	}
}

void Process::test_generateRandomCommands(int limit)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 1);

	for (int i = 0; i < limit; i++)
	{
		auto commandType = static_cast<ICommand::CommandType>(dis(gen));

		if (commandType == ICommand::IO) {
			// Do nothing

		}
		else if (commandType == ICommand::PRINT) {
			String toPrint = "Hello, World!";
			const std::shared_ptr<ICommand> print = std::make_shared<PrintCommand>(this->pid, toPrint);
			this->commandList.push_back(print);
		}

		String toPrint = "Hello, World!";
		const std::shared_ptr<ICommand> print = std::make_shared<PrintCommand>(this->pid, toPrint);
		this->commandList.push_back(print);
	}
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


void Process::executeCurrentCommand() const
{
	this->commandList[this->commandCounter]->execute();
}

void Process::moveToNextLine()
{
	this->commandCounter++;
}

bool Process::isFinished() const
{
	return this->commandCounter == this->commandList.size();
}