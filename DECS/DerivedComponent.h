#pragma once
#include "Component.h"
#include <string>


class DerivedComponent : public Component
{
public:
	DerivedComponent();
	~DerivedComponent();

	DerivedComponent(const DerivedComponent& copy) 
	{ 
		someFloat = copy.someFloat;
		someInteger = copy.someInteger;
		someString = copy.someString;
	};

	void setBelongsToID(int id) override { belongsTo = id; };
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