#pragma once
#include "ICommand.h"
#include "../TypedefRepo.h"

class PrintCommand : public ICommand
{
public:
    PrintCommand(int pid, String& toPrint);

	// Overridden functions
    void execute() override;

private:
    String toPrint;
};
