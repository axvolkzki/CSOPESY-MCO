#include "KeyboardEventHandler.h"
#include <iostream>

KeyboardEventHandler::KeyboardEventHandler() : inputBuffer(""), paused(false), isSchedulerStop(false)
{
}

void KeyboardEventHandler::onKeyDown(char key)
{
    // Pause the scheduler
    pauseScheduler();
    // Add the key to the input buffer
    inputBuffer += key;

    // If the buffer exceeds the length of stopCommand, trim it
    if (inputBuffer.size() > stopSchedulerCommand.size()) {
        inputBuffer.erase(0, inputBuffer.size() - stopSchedulerCommand.size());
    }

    // Check if the buffer matches "scheduler-stop"
    if (inputBuffer == stopSchedulerCommand) {
        isSchedulerStop = true;  // Set stop flag
        std::cout << "Scheduler-stop command received." << std::endl;
        inputBuffer.clear();  // Clear buffer after detecting the command
    }
}

void KeyboardEventHandler::onKeyUp(char key)
{
    
}

void KeyboardEventHandler::pauseScheduler() {
    paused = true;
    std::cout << "Scheduler paused." << std::endl;
}

void KeyboardEventHandler::resumeScheduler() {
    paused = false;
    std::cout << "Scheduler resumed." << std::endl;
}

bool KeyboardEventHandler::isPaused() const {
    return paused;
}

bool KeyboardEventHandler::getIsSchedulerStop() const {
    return isSchedulerStop;
}
