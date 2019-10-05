#include "Driver.h"

DriverManager::DriverManager()
{
	numDrivers = 0;
}

void DriverManager::addDriver(Driver* drv)
{
	drivers[numDrivers] = drv;
	numDrivers++;
}

void DriverManager::activateAll()
{
	for (int i = 0; i < numDrivers; i++)
		drivers[i]->activate();
}