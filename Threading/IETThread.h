#pragma once
#include <iostream>
#include <thread>

//// <summary>
//// Representation of a Java/C# like thread class. The behaviour must be inherited by class that needs to be threated.
//// By: NeilDG
//// </summary>

class IETThread
{
public:
	IETThread() = default;
	~IETThread() = default;

	void start();	// schedules thread execution
	static void sleep(int ms);

protected:
	virtual void run() = 0;	// must be implemented by derived class
};