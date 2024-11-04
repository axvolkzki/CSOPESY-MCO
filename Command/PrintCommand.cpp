#include "PrintCommand.h"
#include "../Threading/IETThread.h"
#include "../TypedefRepo.h"

#include <iostream>
//#include "MessageBuffer.h"

PrintCommand::PrintCommand(int pid, String& toPrint) : ICommand(pid, PRINT)
{
	this->toPrint = toPrint;
}

void PrintCommand::execute()
{
	/*ICommand::execute();*/

	std::cout << "PID " << this->pid << ":" << this->toPrint << std::endl;

	/*std::stringstream msg; msg << String("PID ") << this->pid << ":" this->toPrint << std::endl;*/
	//MessageBuffer::log(msg.str());
}