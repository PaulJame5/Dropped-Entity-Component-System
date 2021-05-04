#pragma once
#include "SpriteComponent.h"
#include "PositionSystem.h"

class SpriteSystem : public decs::System<SpriteComponent>
{
public:
	PositionSystem positionSystem;


	void draw(sf::RenderTarget& target)
	{
		std::vector<SpriteComponent>& list = getDenseList();
		for (int i = 0; i < getNumberOfActiveComponents(); i++)
		{
			if (list[i].isActive() == false)
			{
				continue;
			}

			SpriteComponent& spr = list[i];
			spr.sprite.setPosition(positionSystem.getComponentWithID(spr.belongsToID()).position);
			target.draw(spr.sprite);
		}
	}
};
