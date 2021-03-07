/*
	Dropped-Entity Component System is Created by Paul O'Callghan.
	Component when created need a belongs to id, this is our way of handling Dropped Entities by assinging ids to components 
	instead of components to entities.

	You will need to implement copy constructors in the derived class to use all the fucntionality of the ISystem 
*/
#pragma once

class Component
{
public:
	Component() {}
	virtual ~Component() {};
	
	int belongsToID() { return belongsTo; }
	virtual void setBelongsToID(int id) = 0; 
protected:
	int belongsTo = -1;
};