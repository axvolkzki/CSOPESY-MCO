#pragma once
#include "IKeyboardEvent.h"
#include "../TypedefRepo.h"

#include <atomic>
#include <string>

class KeyboardEventHandler : public IKeyboardEvent
{
public:
	KeyboardEventHandler();
	void onKeyDown(char key) override;
	void onKeyUp(char key) override;

	void pauseScheduler();
	void resumeScheduler();
	bool isPaused() const;
	bool getIsSchedulerStop() const;  // Method to check if stop command was entered

private:
	String inputBuffer;
	std::atomic<bool> paused;
	std::atomic<bool> isSchedulerStop;  // Flag for stop command
	const String stopSchedulerCommand = "scheduler-stop";
};
