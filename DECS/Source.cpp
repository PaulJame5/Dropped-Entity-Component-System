#include <iostream>
#include "DerivedSystem.h"
#include <chrono>
#include <string>

void TestAddDefault(); 
void TestAddPooling();
void TestAddCopy();
void TestRemove();
void TestDelete();
void TestUpdate();

DerivedSystem derivedSystem;
int amount = 100000;

int main()
{
	DerivedSystem::reserveConstantpoolSize(amount+1);
	system("pause");
	TestAddDefault();
	std::cout << "Pool size: " << derivedSystem.getRecycablePoolSize() << std::endl;
	std::cout << "cap: " << derivedSystem.getRecycalblePool().capacity() << std::endl;
	std::cout << "capP: " << DerivedSystem::poolCapacity << std::endl;
	std::cout << "Reserved: " << DerivedSystem::reservedSize << std::endl;
	std::cout << "Reserved: " << derivedSystem.reservedSize << std::endl;

	TestRemove();
	std::cout << "Pool size: " << derivedSystem.getRecycablePoolSize() << std::endl;
	std::cout << "cap: " << derivedSystem.getRecycalblePool().capacity() << std::endl;
	std::cout << "capP: " << DerivedSystem::poolCapacity << std::endl;

	TestAddPooling();
	//derivedSystem.clearRecycablePool();
	std::cout << "Pool size: " << derivedSystem.getRecycablePoolSize() << std::endl;
	std::cout << "cap: " << derivedSystem.getRecycalblePool().capacity() << std::endl;
	std::cout << "capP: " << DerivedSystem::poolCapacity << std::endl;


	TestDelete();
	std::cout << "Entitiy size" << DerivedSystem::getEntities().size() << std::endl;
	std::cout << "Pool size: " << derivedSystem.getRecycablePoolSize() << std::endl;
	std::cout << "cap: " << derivedSystem.getRecycalblePool().capacity() << std::endl;
	std::cout << "capP: " << DerivedSystem::poolCapacity << std::endl;

	TestAddDefault();
	std::cout << "Pool size: " << derivedSystem.getRecycablePoolSize() << std::endl;
	std::cout << "cap: " << derivedSystem.getRecycalblePool().capacity() << std::endl;
	std::cout << "capP: " << DerivedSystem::poolCapacity << std::endl;

	TestDelete();
	std::cout << "Pool size: " << derivedSystem.getRecycablePoolSize() << std::endl;
	std::cout << "cap: " << derivedSystem.getRecycalblePool().capacity() << std::endl;
	std::cout << "capP: " << DerivedSystem::poolCapacity << std::endl;

	TestAddCopy();
	std::cout << "Pool size: " << derivedSystem.getRecycablePoolSize() << std::endl;
	std::cout << "cap: " << derivedSystem.getRecycalblePool().capacity() << std::endl;
	std::cout << "capP: " << DerivedSystem::poolCapacity << std::endl;

	TestUpdate();

	std::cout << "cap: " << derivedSystem.getRecycalblePool().capacity() << std::endl;
	std::cout << "capP: " << DerivedSystem::poolCapacity << std::endl;
	system("pause");
	return 0;
}

void TestAddDefault()
{
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "================== Test Add Default ==================" << std::endl;
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

	// Stuff
	for (int i = 0; i < amount; i++)
	{
		derivedSystem.addComponent(i);
	}

	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Add Default: " << amount << " components took " << milliseconds.count() << " milliseconds." << std::endl;

	std::cout << "======================================================" << std::endl;
}

void TestAddPooling()
{
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "================== Test Add Pooling ==================" << std::endl;
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

	// Stuff
	for (int i = 0; i < amount; i++)
	{
		derivedSystem.addComponent(i);
	}

	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Add Pooling: " << amount << " components took " << milliseconds.count() << " milliseconds." << std::endl;
	std::cout << "======================================================" << std::endl;
}

void TestAddCopy()
{
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "================== Test Add Copy ==================" << std::endl;
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	DerivedComponent derivedComp;

	// Stuff
	for (int i = 0; i < amount; i++)
	{
		derivedSystem.addComponent(i, derivedComp);
	}

	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Add Copy: " << amount << " components took " << milliseconds.count() << " milliseconds." << std::endl;
	std::cout << "======================================================" << std::endl;
}

void TestRemove()
{
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "================== Test Remove ==================" << std::endl;
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

	// Stuff
	for (int i = 0; i < amount; i++)
	{
		derivedSystem.removeComponent(i);
	}

	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Remove: " << amount << " components took " << milliseconds.count() << " milliseconds." << std::endl;
	std::cout << "======================================================" << std::endl;

}

void TestDelete()
{
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "================== Test Delete ==================" << std::endl;
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

	// Stuff
	for (int i = 0; i < amount; i++)
	{
		derivedSystem.destroyComponent(i);
	}

	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Destroy: " << amount << " components took " << milliseconds.count() << " milliseconds." << std::endl;
	std::cout << "======================================================" << std::endl;
}


void TestUpdate()
{
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "================== Test Update ==================" << std::endl;
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	derivedSystem.update();
	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Update: " << amount << " components took " << milliseconds.count() << " milliseconds." << std::endl;
	std::cout << "======================================================" << std::endl;
}