#include <iostream>
#include "DerivedSystem.h"
//#include "Entity.h"
#include "Timer.h"
#include "decs.h"
#include <string>

void TestAddDefault(); 
void TestAddPooling();
void TestAddCopy();
void TestRemove();
void TestDelete();
void TestUpdate();


DerivedSystem derivedSystem;
DerivedSystem derivedSystem2;
int amountOfComponents = 1000000;
int amountOfTests = 100;

int main()
{
	system("pause");
	Timer addDefault = Timer("Add Default"); 
	Timer remove =Timer("Remove");
	Timer addFromPool =Timer("Add From Pool");
	Timer deleteComponent =Timer("Delete"); 
	Timer passCopyTimer =Timer("Construct Copy"); 
	Timer updateTimer =Timer("Update");



	for (int i = 0; i < amountOfTests; i++)
	{
		// Remove before starting again
		TestDelete();

		addDefault.Start();
		TestAddDefault();
		addDefault.Stop();

		remove.Start();
		TestRemove();
		remove.Stop();

		addFromPool.Start();
		TestAddPooling();
		addFromPool.Stop();

		deleteComponent.Start();
		TestDelete();
		deleteComponent.Stop();

		passCopyTimer.Start();
		TestAddCopy();
		passCopyTimer.Stop();

		updateTimer.Start();
		TestUpdate();
		updateTimer.Stop();

		std::cout << "finished iter: " << i << std::endl;
	}
		addDefault.~Timer();
		remove.~Timer();
		addFromPool.~Timer();
		deleteComponent.~Timer();
		passCopyTimer.~Timer();
		updateTimer.~Timer();


	system("pause");
	return 0;
}

void TestAddDefault()
{
	for (int i = 0; i < amountOfComponents; i++)
	{
		derivedSystem.addComponent(i);
	}
}

void TestAddPooling()
{
	for (int i = 0; i < amountOfComponents; i++)
	{
		derivedSystem.addComponent(i);
	}

}

void TestAddCopy()
{
	// Stuff
	for (int i = 0; i < amountOfComponents; i++)
	{
		DerivedComponent derivedComp;
		derivedSystem.addComponent(i, derivedComp);
	}
}

void TestRemove()
{
	// Stuff
	for (int i = 0; i < amountOfComponents; i++)
	{
		derivedSystem.removeComponents(i);
	}
}

void TestDelete()
{
	// Stuff
	for (int i = 0; i < amountOfComponents; i++)
	{
		derivedSystem.destroyComponents(i);
	}

}


void TestUpdate()
{
	derivedSystem.update();
}
