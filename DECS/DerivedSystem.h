#pragma once
//#include "ISystem.h"
#include "decs.h"
#include "DerivedComponent.h"

class DerivedSystem : public ISystem<DerivedComponent>
{
public:
	DerivedSystem() {}
	~DerivedSystem() {}

	void update() 
	{
		typename std::map<int, std::vector<DerivedComponent> >::iterator it = entities.begin();
		for (; it != entities.end(); it++)
		{
			for (int i = 0; i < it->second.size(); i++)
			{
				it->second.at(i).setValuesOfSomeVariables();
			}
		}
	}

private:

};