#include "AConsole.h"
#include "../TypedefRepo.h"
#include <cstdint>

// Constructor
AConsole::AConsole(String name) : name(name)
{
}

// Returns the name of the console
String AConsole::getName()
{
	return name;
}