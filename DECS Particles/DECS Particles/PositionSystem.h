#pragma once

#include "decs.h"
#include "PositionComponent.h"
#include <iostream>

class PositionSystem : public decs::System<PositionComponent>
{
public:
	PositionSystem() {};
	~PositionSystem() {};
};