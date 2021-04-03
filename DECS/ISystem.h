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
	ISystem() { /*entities.clear();*/ };
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
	static void destroyComponentAt(int entityId, int position);

	static bool hasComponent(int entityId);

	// Returns first element null/blank if empty
	static T& getComponent(int entityId);

	// Returns list of components type T attached to entity
	static std::vector<T>& getComponents(int entityId);

	static int getRecycablePoolSize();

	static std::map<int, std::vector<T>>& getEntities();
	static std::vector<T>& getRecycalblePool();

	static void reserveConstantpoolSize(int size);
	static void shrinkPoolCapacity();
	static int poolCapacity;
	static bool reservedSize;

protected:
	static std::map<int, std::vector<T>> entities;
	static std::vector<T> recycablePool;

private:
	static void poolIncrementCalculation();
	static int poolSize;
	static int poolCapacityHalf;
};

template<class T>
ISystem<T>::~ISystem() {}


template<class T>
std::map<int, std::vector<T>> ISystem<T>::entities = std::map<int, std::vector<T>>();

template<class T>
std::vector<T> ISystem<T>::recycablePool = {};

template<class T>
int ISystem<T>::poolSize = 0;

template<class T>
int ISystem<T>::poolCapacity = 0;

template<class T>
int ISystem<T>::poolCapacityHalf = 0;

template<class T>
bool ISystem<T>::reservedSize = false;



template<class T>
inline void ISystem<T>::addComponent(int entityId)
{
	if (recycablePool.empty() == false)
	{
		entities[entityId].push_back(recycablePool.back());
		recycablePool.back().setBelongsToID(entityId);
		recycablePool.pop_back();
		poolSize--;

		if (reservedSize)
		{
			return;
		}

		// Free up memory 
		if (poolSize <= poolCapacityHalf - 1)
		{
			recycablePool.shrink_to_fit();
			poolCapacity = recycablePool.capacity();
			poolCapacityHalf = poolCapacity / 2;
		}
		return;
	}
	T newClassComponent;
	newClassComponent.setBelongsToID(entityId);
	entities[entityId].push_back(newClassComponent);
}

template<class T>
inline void ISystem<T>::addComponent(int entityId, T newComponent)
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

	typename std::map<int, std::vector<T> >::iterator it = entities.find(entityId);

	if (reservedSize)
	{
		if (poolSize > poolCapacity)
		{
			it->second.erase(it->second.begin());
			if (it->second.size() == 0)
			{
				entities.erase(entityId);
			}
			return;
		}
	}
	recycablePool.push_back(it->second.front());

	it->second.front().setBelongsToID(-1);
	it->second.erase(it->second.begin());


	poolIncrementCalculation();

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
		recycablePool.push_back(it->second.at(0));
		it->second.erase(it->second.begin());
		poolIncrementCalculation();
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


// Removes first element found with component without assigning it to a pool
template<class T>
inline void ISystem<T>::destroyComponentAt(int entityId, int position)
{
	if (!hasComponent(entityId))
	{
		return;
	}

	typename std::map<int, std::vector<T> >::iterator it = entities.find(entityId);

	if (it->second.size() < position + 1)
	{
		return;
	}

	it->second.erase(it->second.at(position));

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
inline std::vector<T>& ISystem<T>::getRecycalblePool()
{
	return recycablePool;
}

template<class T>
inline void ISystem<T>::reserveConstantpoolSize(int size)
{
	recycablePool.reserve(size);
	reservedSize = true;
	poolCapacity = recycablePool.capacity();
	
}

template<class T>
inline void ISystem<T>::shrinkPoolCapacity()
{
	recycablePool.shrink_to_fit();
}

template<class T>
inline void ISystem<T>::poolIncrementCalculation()
{

	++poolSize;
	if (poolCapacity < poolSize)
	{
		poolCapacity = recycablePool.capacity();
		poolCapacityHalf = poolCapacity / 2;
	}
}
