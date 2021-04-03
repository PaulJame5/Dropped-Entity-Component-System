#pragma once
#include "Component.h"
#include <string>


class DerivedComponent : public Component
{
public:
	DerivedComponent();
	~DerivedComponent();

	void init(int belongsToId) override { setBelongsToID(belongsToId); }

	DerivedComponent(const DerivedComponent& copy) 
	{ 
		someFloat = copy.someFloat;
		someInteger = copy.someInteger;
		someString = copy.someString;
	};

	void setValuesOfSomeVariables()
	{

		for (int i = 0; i < 200; i++)
		{
			arrayType[i] = i * 2 + 5 % 2 / 3;
		}
		someInteger = 50;
		someString = "slkdjfhlkjaefhlkjsfhlkjsdfhlkasj";
		someFloat = 12121.77839739f;
	}

private:
	int someInteger = 50;
	std::string someString = "slkdjfhlkjaefhlkjsfhlkjsdfhlkasj";
	float someFloat = 12121.77839739f;
	int arrayType[200];
};

DerivedComponent::DerivedComponent()
{
	for (int i = 0; i < 200; i++)
	{
		arrayType[i] = i * 2 + 5 % 2 / 3;
	}
}

DerivedComponent::~DerivedComponent()
{
}