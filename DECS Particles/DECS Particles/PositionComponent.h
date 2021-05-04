#pragma once
#include "decs.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include "PhysicsSystem.h"

class PositionComponent : public decs::Component
{
public:
	PositionComponent() {  };
	~PositionComponent() {  };
	PhysicsSystem physicsSystem;
	sf::Vector2f position;

	void initialise() override 
	{
		position = { 0, 0 };
	};

	void update() override
	{
		position += physicsSystem.getComponentWithID(belongsTo).velocity * decs::World::getDeltaTime();
	}

};