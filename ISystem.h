/*
	Dropped-Entity Component System is Created by Paul O'Callghan.
	This is the system used for adding and removing components of class template type t. Systems should inherit from this class using
	Polymorhism to extend their system classes.
*/

#pragma once
#include "Component.h"
#include <map>
#include <list>
#include <vector>

template<class T>
class ISystem
{
public:
	ISystem();
	~ISystem();

	// Adds component with default values
	static void addComponent(int entityId);

	// Adds component with values to entity id
	static void addComponent(int entityId, T& t);

	//// Removes component from entity and places it in a recyclable pool
	static void removeComponent(int entityId);
	static void removeComponents(int entityId);

	// Destroys / Deletes component from memeory
	static void destroyComponent(int entityId);
	static void destroyComponents(int entityId);
	static void destroyComponentAt(int entityId, int position);

	static bool hasComponent(int entityId);

	// Returns first element null/blank if empty
	static T& getComponent(int entityId);

	// Returns list of components type T attached to entity
	static std::vector<T&>& getComponents(int entityId);

	static int getRecycablePoolSize();

	static std::map<int, std::vector<T&>>& getEntities();
	static std::vector<T*>& getRecycalblePool();


protected:
	static std::map<int, std::vector<T>> entities;
	static std::list<T> recycablePool;

private:

};

template<class T>
inline ISystem<T>::ISystem()
{
}

template<class T>
ISystem<T>::~ISystem() 
{ 
}

template<class T>
std::map<int, std::vector<T>> ISystem<T>::entities = { {} };

template<class T>
std::list<T> ISystem<T>::recycablePool = std::list<T>();


template<class T>
inline void ISystem<T>::addComponent(int entityId)
{
	if (recycablePool.empty() == false)
	{
		recycablePool.front().setBelongsToID(entityId);
		entities[entityId].push_back(recycablePool.front()); 
		recycablePool.erase(recycablePool.begin());
		return;
	}
	
	T newObject;
	newObject.setBelongsToID(entityId);
	entities[entityId].push_back(newObject);
}

template<class T>
inline void ISystem<T>::addComponent(int entityId, T& newComponent)
{
	newComponent.setBelongsToID(entityId);
	entities[entityId].push_back(newComponent);
}

// Removes first component if there is more than 1
template<class T>
inline void ISystem<T>::removeComponent(int entityId)
{
	if (!hasComponent(entityId))
	{
		return;
	}

	recycablePool.push_back(entities[entityId].back());
	entities[entityId].erase(entities[entityId].end() - 1);

	if (entities[entityId].size() == 0)
	{
		entities[entityId].shrink_to_fit();
		entities.erase(entityId);
	}
}

// Deletes all Components
template<class T>
inline void ISystem<T>::removeComponents(int entityId)
{
	if (!hasComponent(entityId))
	{
		return;
	}

	while (entities[entityId].size() > 0)
	{
		recycablePool.push_back(entities[entityId].at(0));
		entities[entityId].erase(entities[entityId].begin());
	}
	entities[entityId].shrink_to_fit();
	entities.erase(entityId);
}

// Deletes first element found with component without assigning it to a pool
template<class T>
inline void ISystem<T>::destroyComponent(int entityId)
{
	if (!hasComponent(entityId))
	{
		return;
	}

	entities[entityId].erase(entities[entityId].end() - 1);

	if (entities[entityId].size() == 0)
	{
		entities[entityId].shrink_to_fit();
		entities.erase(entityId);
	}
}


// Deletes element at position
template<class T>
inline void ISystem<T>::destroyComponentAt(int entityId, int position)
{
	if (!hasComponent(entityId))
	{
		return;
	}

	if (entities[entityId].size() < position + 1)
	{
		return;
	}

	entities[entityId].erase(entities[entityId].at(position));

	if (entities[entityId].size() == 0)
	{
		entities.erase(entityId);
	}
}

// Deletes all components and removes entity from map without assigning objects to a pool 
template<class T>
inline void ISystem<T>::destroyComponents(int entityId)
{
	if (!hasComponent(entityId))
	{
		return;
	}

	entities[entityId].erase(entities[entityId].begin(), entities[entityId].end());
	entities[entityId].shrink_to_fit();
	entities.erase(entityId);
}

template<class T>
inline bool ISystem<T>::hasComponent(int entityId)
{
	typename std::map<int, std::vector<T> >::iterator it = entities.find(entityId);

	if (it == entities.end())
	{
		return false;
	}
	return true;
}

// Returns first component
template<class T>
inline T& ISystem<T>::getComponent(int entityId)
{
	if (!hasComponent(entityId))
	{
		addComponent(entityId);
	}
	
	return entities[entityId].at(0);
}

// Returns all components
template<class T>
inline std::vector<T&>& ISystem<T>::getComponents(int entityId)
{
	if (!hasComponent(entityId))
	{
		addComponent(entityId);
	}

	return entities[entityId];
}

template<class T>
inline int ISystem<T>::getRecycablePoolSize()
{
	return getRecycalblePool().size();
}

template<class T>
inline std::map<int, std::vector<T&>>& ISystem<T>::getEntities()
{
	return entities;
}

template<class T>
inline std::vector<T*>& ISystem<T>::getRecycalblePool()
{
	return recycablePool;
}