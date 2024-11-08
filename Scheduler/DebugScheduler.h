#pragma once
#include <memory>
#include <queue>
#include <sstream>

#include "AScheduler.h"
#include "../Process/Process.h"


class DebugScheduler :
    public AScheduler
{
public:
	DebugScheduler();
	/*~DebugScheduler();*/

	void init() override;
	void execute() override;

private:
	std::shared_ptr<Process> currentProcess = nullptr;
};

