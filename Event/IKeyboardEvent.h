#pragma once
#include "../TypedefRepo.h"

class IKeyboardEvent
{
public:
	virtual void onKeyDown(char key) = 0;					// Pure virtual function to handle the key down event
	virtual void onKeyUp(char key) = 0;
};

//inline void IKeyboardEvent::onKeyDown(char key)
//{
//}
//
//inline void IKeyboardEvent::onKeysUp(char key)
//{
//}