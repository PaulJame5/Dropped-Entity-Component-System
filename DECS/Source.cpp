#include "dcs.h"
#include "TestSystem.h"
#include "Timer.h"

void TestAddDefault();
void TestAddPooling();
void TestAddCopy();
void TestRemove();
void TestDelete();
void TestUpdate();
void TestRandomAccess();

TestSystem testSystem;

int amountOfComponents = 100000;
int amountOfTests = 100;

int main()
{
	Timer addDefault = Timer("Add Default");
	Timer removeWithID = Timer("Remove");
	Timer addFromPool = Timer("Add From Pool");
	Timer deleteComponent = Timer("Delete");
	Timer passCopyTimer = Timer("Construct Copy");
	Timer updateTimer = Timer("Update");
	Timer randomAccessTimer = Timer("Random Access");


	testSystem.reserveComponentCapacity(amountOfComponents);
	testSystem.reserveIDCapacity(amountOfComponents);
	for (int i = 0; i < amountOfTests; i++)
	{

		passCopyTimer.Start();
		TestAddCopy();
		passCopyTimer.Stop();

		deleteComponent.Start();
		TestDelete();
		deleteComponent.Stop();

		decs::World::destroyOrphanedEntities();
		decs::World::destroyMarked();

		addDefault.Start();
		TestAddDefault();
		addDefault.Stop();

		removeWithID.Start();
		TestRemove();
		removeWithID.Stop();

		addFromPool.Start();
		TestAddPooling();
		addFromPool.Stop();

		updateTimer.Start();
		TestUpdate();
		updateTimer.Stop();

		randomAccessTimer.Start();
		TestRandomAccess();
		randomAccessTimer.Stop();

		decs::World::destroyAllEntities(false);
		decs::World::destroyMarked();
		testSystem.clear();


		std::cout << "finished iter: " << i << std::endl;
	}
	addDefault.PrintResults();
	removeWithID.PrintResults();
	addFromPool.PrintResults();
	deleteComponent.PrintResults();
	passCopyTimer.PrintResults();
	updateTimer.PrintResults();
	randomAccessTimer.PrintResults();


	system("pause");
}

void TestAddDefault()
{
	for (int i = 0; i < amountOfComponents; i++)
	{
		testSystem.addComponentWithID(decs::World::createNewID());
	}
}

void TestAddPooling()
{
	for (int i = 0; i < amountOfComponents; i++)
	{
		// We know the ids are between 0 & 99999
		testSystem.addComponentWithID(i);
	}

}

void TestAddCopy()
{
	for (int i = 0; i < amountOfComponents; i++)
	{
		TestComponent copy;
		testSystem.addComponentValuesWithID(decs::World::createNewID(), copy);
	}
}

void TestRemove()
{
	for (int i = 0; i < amountOfComponents; i++)
	{
		testSystem.removeAllComponentsWithID(i);
	}
}

void TestDelete()
{
	for (int i = 0; i < amountOfComponents; i++)
	{
		testSystem.destroyAllComponentsWithID(i);
	}
}


void TestUpdate()
{
	testSystem.update();
}

void TestRandomAccess()
{
	for (int i = 0; i < amountOfComponents; i++)
	{
		testSystem.getPtrComponentWithID(i);
	}
}