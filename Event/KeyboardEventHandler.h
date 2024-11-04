#pragma once
#include "IKeyboardEvent.h"
#include "../TypedefRepo.h"


class KeyboardEventHandler :
    public IKeyboardEvent
{
public:
	KeyboardEventHandler();
	void onKeyDown(char key) override;
	void onKeyUp(char key) override;

private:
	String inputBuffer;
	const String stopSchedulerCommand = "scheduler-stop";
};

