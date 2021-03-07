#pragma once
#include "ISystem.h"
#include "DerivedComponent.h"

class DerivedSystem : public ISystem<DerivedComponent>
{
public:
	DerivedSystem() {}
	~DerivedSystem() {}

private:

};