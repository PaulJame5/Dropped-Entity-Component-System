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
	//virtual Component(const& Component);

	virtual void init(int belongsToID) = 0;
	
	int belongsToID() { return belongsTo; }
	void setBelongsToID(int id) { belongsTo = id; };
	void setActive(bool active) { isActive = active; }

protected:
	int belongsTo = -1;
	bool isActive = true;
};