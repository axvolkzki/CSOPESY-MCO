#include "KeyboardEventHandler.h"
#include <iostream>

KeyboardEventHandler::KeyboardEventHandler() : inputBuffer("")
{
}

void KeyboardEventHandler::onKeyDown(char key)
{
	inputBuffer += key;

    // If the buffer exceeds the length of stopCommand, trim it
    if (inputBuffer.size() > stopSchedulerCommand.size()) {
        inputBuffer.erase(0, inputBuffer.size() - stopSchedulerCommand.size());
    }

    // Check if the buffer matches "scheduler-stop"
    if (inputBuffer == stopSchedulerCommand) {
        /*isSchedulerStop = true;*/
        std::cout << "Scheduler-stop command received." << std::endl;
        inputBuffer.clear();  // Clear buffer after detecting the command
    }
}

void KeyboardEventHandler::onKeyUp(char key)
{
	std::cout << "Keys up: " << key << std::endl;
}
