#pragma once
#include "../Config/GlobalConfig.h"
#include "../Threading/IETThread.h"


class ICommand
{
public:
	// Enum
	enum CommandType {
		IO,
		PRINT
	};

	ICommand(int pid, CommandType commandType);
	CommandType getCommandType();

	// Overridden functions
	virtual void execute() = 0;

	virtual ~ICommand() = default;


protected:
	int pid;
	CommandType commandType;
};

inline ICommand::CommandType ICommand::getCommandType()
{
	return commandType;
}