#pragma once
#include "decs.h";
#include <SFML/Graphics.hpp>

const float GRAVITY = 9.8f;
class PhysicsComponent : public decs::Component
{
public:
	sf::Vector2f velocity;
	float gravityScale;

	void initialise() override
	{
		velocity = { 0, 0};
		gravityScale = 200.0f;
	}


	void update() override
	{
		velocity.y += (GRAVITY * gravityScale * decs::World::getDeltaTime());
	}
};