#pragma once
#include "dcs.h"
#include "TestComponent.h"
#include "T2.h"

class TestSystem : public decs::System<TestComponent>
{
public:
	TestSystem();
	~TestSystem();

private:

};

TestSystem::TestSystem()
{
}

TestSystem::~TestSystem()
{
}