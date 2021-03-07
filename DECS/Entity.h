/*
	Dropped-Entity Component System is Created by Paul O'Callghan.
	Entity for DECS doesn't hold any value or components instead allows the user to create ids or create a custom id
	that will store in a vector and be checked against when creating a new id.
	Entity probably isn't even the best name for this as it is more of identifier added to systems and components

	Custom assignable ids are checked if you want to initialise specific entities in your program and have the id unusable by other ids
	perfect for if you have const PLAYER = 9999, you don't want to accidentally add an ai component to the PLAYER id
*/

#pragma once
#include <algorithm>
#include <vector>

class Entity
{
public:
	~Entity() {};

	static int createNewEnitityID() 
	{ 
		while (std::find(customAssignedIds.begin(), customAssignedIds.end(), nextAssignableID) == customAssignedIds.end())
		{
			nextAssignableID++;
		}
		return nextAssignableID++; 
	};
	static int createNewEnitityWithID(int id) 
	{ 
		if (std::find(customAssignedIds.begin(), customAssignedIds.end(), id) != customAssignedIds.end())
		{
			return -1;
		}
		customAssignedIds.push_back(id);
		return id; 
	};
private:
	Entity();
	static int nextAssignableID;
	static std::vector<int> customAssignedIds;
};

