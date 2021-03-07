/*
	Dropped-Entity Component System is Created by Paul O'Callghan.
	This is the system used for adding and removing components of class template type t. Systems should inherit from this class using
	Polymorhism to extend their system classes.
*/


#pragma once
#include "Component.h"
#include <map>
#include <queue>
#include <vector>


template<class T>
class ISystem
{
public:
	ISystem() {};
	virtual inline ~ISystem() = 0;

	// Adds component with default values
	static void addComponent(int entityId);

	// Adds component with values to entity id
	static void addComponent(int entityId, T t);

	// Removes component from entity and places it in a recyclable pool
	static void removeComponent(int entityId);
	static void removeComponents(int entityId);

	// Destroys / Deletes component from memeory
	static void destroyComponent(int entityId);
	static void destroyComponents(int entityId);

	static bool hasComponent(int entityId);

	// Returns first element null/blank if empty
	static T& getComponent(int entityId);

	// Returns list of components type T attached to entity
	static std::vector<T>& getComponents(int entityId);

	static int getRecycablePoolSize();

	static std::map<int, std::vector<T>>& getEntities();
	static std::queue<T>& getRecycalblePool();

protected:
	static std::map<int, std::vector<T>> entities;
	static std::queue<T> recycablePool;
};

template<class T>
ISystem<T>::~ISystem() {}


template<class T>
std::map<int, std::vector<T>> ISystem<T>::entities = std::map<int, std::vector<T>>();

template<class T>
std::queue<T> ISystem<T>::recycablePool = {};


template<class T>
inline void ISystem<T>::addComponent(int entityId)
{
	if (recycablePool.empty() == false)
	{
		entities[entityId].push_back(recycablePool.front());
		recycablePool.pop();
		return;
	}
	T newClassComponent;

	entities[entityId].push_back(newClassComponent);
}

template<class T>
inline void ISystem<T>::addComponent(int entityId, T newComponent)
{
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

	typename std::map<int, std::vector<T> >::iterator it = entities.find(entityId);
	it->second.at(0).setBelongsToID(-1);
	recycablePool.push(it->second.at(0));
	it->second.erase(it->second.begin());

	if (it->second.size() == 0)
	{
		entities.erase(entityId);
	}
}

template<class T>
inline void ISystem<T>::removeComponents(int entityId)
{
	if (!hasComponent(entityId))
	{
		return;
	}

	typename std::map<int, std::vector<T> >::iterator it = entities.find(entityId);

	while (it->second.size() > 0)
	{
		recycablePool.push(it->second.at(0));
		it->second.erase(it->second.begin());
	}

	if (it->second.size() == 0)
	{
		entities.erase(entityId);
	}
}

// Removes first element found with component without assigning it to a pool
template<class T>
inline void ISystem<T>::destroyComponent(int entityId)
{
	if (!hasComponent(entityId))
	{
		return;
	}

	typename std::map<int, std::vector<T> >::iterator it = entities.find(entityId);
	it->second.erase(it->second.begin());


	if (it->second.size() == 0)
	{
		entities.erase(entityId);
	}
}

// Reoves all components and removes entity from map without assigning objects to a pool 
template<class T>
inline void ISystem<T>::destroyComponents(int entityId)
{
	if (!hasComponent(entityId))
	{
		return;
	}

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

template<class T>
inline T& ISystem<T>::getComponent(int entityId)
{
	if (!hasComponent(entityId))
	{
		addComponent(entityId);
	}

	typename std::map<int, std::vector<T> >::iterator it = entities.find(entityId);
	return it->second.at(0);
}

template<class T>
inline std::vector<T>& ISystem<T>::getComponents(int entityId)
{
	if (!hasComponent(entityId))
	{
		addComponent(entityId);
	}

	typename std::map<int, std::vector<T> >::iterator it = entities.find(entityId);
	return it->second;
}

template<class T>
inline int ISystem<T>::getRecycablePoolSize()
{
	return getRecycalblePool().size();
}

template<class T>
inline std::map<int, std::vector<T>>& ISystem<T>::getEntities()
{
	return entities;
}

template<class T>
inline std::queue<T>& ISystem<T>::getRecycalblePool()
{
	return recycablePool;
}
