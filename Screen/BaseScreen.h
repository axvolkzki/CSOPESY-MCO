#pragma once
#include <memory>
#include "../TypedefRepo.h"
#include "../Console/AConsole.h"
#include "../Process/Process.h"

class BaseScreen : public AConsole				// BaseScreen is a subclass of AConsole; inherits from AConsole; actual implementation of AConsole
{
public:
	BaseScreen(std::shared_ptr<Process> process, String processName);		// Constructor
	void onEnabled() override;												// Override the onEnabled function of AConsole
	void display() override;												// Override the display function of AConsole
	void process() override;												// Override the process function of AConsole

private:
	std::shared_ptr<Process> attachedProcess;								// The process attached to the screen; class the represents the process; for every base screen, there is a process that contains another set of commands
	bool refreshed = false;													// The refreshed variable
	void displayProcessInfo();												// Display the process information
};

