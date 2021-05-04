#pragma once
#include "decs.h"
#include "PositionSystem.h"


class SpriteComponent : public decs::Component
{
public:
	sf::CircleShape sprite = sf::CircleShape(3);


	void initialise() override
	{
		sprite.setFillColor(sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255));
	}


};

//sf::CircleShape SpriteComponent::sprite = sf::CircleShape(3);