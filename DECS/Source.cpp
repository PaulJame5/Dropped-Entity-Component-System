#include <iostream>
#include "DerivedSystem.h"
#include <chrono>

void TestAddDefault(); 
void TestAddPooling();
void TestAddCopy();
void TestRemove();
void TestDelete();

DerivedSystem derivedSystem;
int amount = 100000;

int main()
{
	system("pause");
	TestAddDefault();
	std::cout << "Pool size: " << derivedSystem.getRecycablePoolSize() << std::endl;
	TestRemove();
	std::cout << "Pool size: " << derivedSystem.getRecycablePoolSize() << std::endl;
	TestAddPooling();
	std::cout << "Pool size: " << derivedSystem.getRecycablePoolSize() << std::endl;
	TestDelete();
	std::cout << "Pool size: " << derivedSystem.getRecycablePoolSize() << std::endl;
	TestAddDefault();
	std::cout << "Pool size: " << derivedSystem.getRecycablePoolSize() << std::endl;
	TestDelete();
	std::cout << "Pool size: " << derivedSystem.getRecycablePoolSize() << std::endl;
	TestAddCopy();
	std::cout << "Pool size: " << derivedSystem.getRecycablePoolSize() << std::endl;
	system("pause");
	return 0;
}

void TestAddDefault()
{
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

	// Stuff
	for (int i = 0; i < amount; i++)
	{
		derivedSystem.addComponent(i);
	}

	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Add Default: " << amount << " components took " << milliseconds.count() << " milliseconds." << std::endl;
}

void TestAddPooling()
{
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

	// Stuff
	for (int i = 0; i < amount; i++)
	{
		derivedSystem.addComponent(i);
	}

	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Add Pooling: " << amount << " components took " << milliseconds.count() << " milliseconds." << std::endl;
}

void TestAddCopy()
{
	DerivedComponent derivedComp;
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

	// Stuff
	for (int i = 0; i < amount; i++)
	{
		derivedSystem.addComponent(i, derivedComp);
	}

	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Add Copy: " << amount << " components took " << milliseconds.count() << " milliseconds." << std::endl;
}

void TestRemove()
{
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

	// Stuff
	for (int i = 0; i < amount; i++)
	{
		derivedSystem.removeComponent(i);
	}

	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Remove: " << amount << " components took " << milliseconds.count() << " milliseconds." << std::endl;

}

void TestDelete()
{
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

	// Stuff
	for (int i = 0; i < amount; i++)
	{
		derivedSystem.destroyComponent(i);
	}

	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Destroy: " << amount << " components took " << milliseconds.count() << " milliseconds." << std::endl;
}