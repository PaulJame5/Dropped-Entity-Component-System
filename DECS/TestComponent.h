#pragma once
#include "dcs.h"
#include <string>

class TestComponent : public decs::Component
{
public:
	TestComponent();
	~TestComponent();

	void initialise() override 
	{  
		setValuesOfSomeVariables();
	}


	void update() override
	{
		setValuesOfSomeVariables();
	}

	

	void setValuesOfSomeVariables()
	{
		for (int index = 0; index < 200; index++)
		{
			arrayType[index] = index * 2 + 5 % 2 / 3;
		}
		someInteger = 50;
		someString = "slkdjfhlkjaefhlkjsfhlkjsdfhlkasj";
		someFloat = 12121.77839739f;
	}

private:
	int someInteger;
	std::string someString;
	float someFloat;
	int arrayType[200];
};

inline TestComponent::TestComponent()
{

}


inline TestComponent::~TestComponent()
{
}