/*
	Single Header include file for DECS
*/
#pragma once

#pragma once
#include <map>
#include <list>
#include <vector>
#include <iostream>

class Entity;

class Component
{
public:
	Component();
	virtual ~Component() {};
	virtual void initialise() = 0;

	int belongsToID();
	void setBelongsToID(int id);
	void setActive(bool active);
	bool isActive();

protected:
	int belongsTo = -1;
	bool activeSelf = true;
};

inline Component::Component()
{

}

inline int Component::belongsToID()
{
	return belongsTo;
}

inline void Component::setBelongsToID(int id)
{
	belongsTo = id;
}

inline void Component::setActive(bool active) 
{ 
	activeSelf = active;
}
inline bool Component::isActive()
{
	return activeSelf;
}

// End Component
class ISystemBase
{
public:
	ISystemBase();
	~ISystemBase();

	virtual void removeComponents(int entityId) = 0;

	virtual void destroyComponents(int entityId) = 0;

	int getSystemID() { return systemID; };

private:
	static int systemID;
	static int assignableSystemID;
};
int ISystemBase::assignableSystemID = 0;
int ISystemBase::systemID = -1;

inline ISystemBase::ISystemBase()
{
	if (systemID != -1)
	{
		return;
	}
	systemID = assignableSystemID++;
}

inline ISystemBase::~ISystemBase()
{

}
// End ISystemBase

//===================================================================

template<class T>
class ISystem : ISystemBase
{
public:
	ISystem();
	~ISystem(); 

	// We do not want our system to be cloneable
	ISystem(ISystem& other) = delete;

	// Adds component with default values
	void addComponent(int entityId);

	// Adds component with values to entity id
	void addComponent(int entityId, T& t);

	//// Removes component from entity and places it in a recyclable pool
	void removeComponent(int entityId);
	void removeComponents(int entityId);

	// Destroys / Deletes component from memeory
	void destroyComponent(int entityId);
	void destroyComponents(int entityId);
	void destroyComponentAt(int entityId, int position);

	bool hasComponent(int entityId);

	// Returns first element null/blank if empty
	T& getComponent(int entityId);

	// Returns list of components type T attached to entity
	std::vector<T>& getComponents(int entityId);


	std::map<int, std::vector<T>>& getEntities();

	std::list<T>& getRecycalblePool();
	int getPoolSize();
	void resetPool();

protected:
	static std::map<int, std::vector<T>> entities;
	static std::list<T> recycablePool;

private:

};

template<class T>
inline ISystem<T>::ISystem()
{
	//ISystemBase& self = *this;
	Entity::addSystem(*this);
}

template<class T>
inline ISystem<T>::~ISystem()
{
}

template<class T>
std::map<int, std::vector<T>> ISystem<T>::entities = std::map<int, std::vector<T>>();

template<class T>
std::list<T> ISystem<T>::recycablePool = std::list<T>();

template<class T>
inline void ISystem<T>::addComponent(int entityId)
{
	std::vector<T>& componentList = entities[entityId];
	if (recycablePool.empty() == false)
	{
		recycablePool.front().setBelongsToID(entityId);
		componentList.emplace_back(std::move(recycablePool.front()));
		recycablePool.pop_front();
		return;
	}

	componentList.emplace_back();
	componentList.back().setBelongsToID(entityId);
}

template<class T>
inline void ISystem<T>::addComponent(int entityId, T& newComponent)
{
	newComponent.setBelongsToID(entityId);
	entities[entityId].emplace_back(newComponent);
}

// Removes first component if there is more than 1
template<class T>
inline void ISystem<T>::removeComponent(int entityId)
{
	if (!hasComponent(entityId))
	{
		return;
	}
	std::vector<T>& componentList = entities[entityId];

	recycablePool.emplace_back(std::move(componentList.back()));
	componentList.pop_back();
	componentList.shrink_to_fit();

	if (componentList.size() < 1)
	{
		entities.erase(entityId);

		if (entities.size() == 0)
		{
			entities = std::map<int, std::vector<T>>();
		}
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

	// Cache optimisation
	std::vector<T>& componentList = entities[entityId];

	while (componentList.size() > 0)
	{
		recycablePool.emplace_back(std::move(componentList.back()));
		componentList.erase(componentList.end() - 1);
	}
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
	// Cache optimisation
	std::vector<T>& componentList = entities[entityId];

	componentList.erase(componentList.end() - 1);

	if (componentList.size() == 0)
	{
		componentList.clear();
		componentList.shrink_to_fit();
		entities.erase(entityId);

	}
	if (entities.size() == 0)
	{
		entities.clear();
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
	// Cache optimisation
	std::vector<T>& componentList = entities[entityId];

	if (componentList.size() < position + 1)
	{
		return;
	}

	componentList.erase(componentList.at(position));

	if (componentList.size() == 0)
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
	// Cache optimisation
	std::vector<T>& componentList = entities[entityId];

	componentList.erase(componentList.begin(), componentList.end());
	componentList.shrink_to_fit();
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
inline std::vector<T>& ISystem<T>::getComponents(int entityId)
{
	if (!hasComponent(entityId))
	{
		addComponent(entityId);
	}

	return entities[entityId];
}

template<class T>
inline int ISystem<T>::getPoolSize()
{
	return getRecycalblePool().size();
}

template<class T>
inline std::map<int, std::vector<T>>& ISystem<T>::getEntities()
{
	return entities;
}

template<class T>
inline std::list<T>& ISystem<T>::getRecycalblePool()
{
	return recycablePool;
}

template<class T>
inline void ISystem<T>::resetPool()
{
	recycablePool.resize(0);
}


/*
	Entiy class and it's implementations
*/
class Entity
{
public:
	~Entity() {};

	static int createNewEnitityID(bool immediate = false)
	{
		if (immediate)
		{
			return nextAssignableID++;
		}
		while (std::find(customAssignedIds.begin(), customAssignedIds.end(), nextAssignableID) == customAssignedIds.end())
		{
			nextAssignableID++;
		}
		return nextAssignableID++;
	};
	static int createNewEnitityWithID(int id, bool immediate = false)
	{
		if (immediate)
		{
			customAssignedIds.emplace_back(id);
			return id;
		}
		if (std::find(customAssignedIds.begin(), customAssignedIds.end(), id) != customAssignedIds.end())
		{
			return -1;
		}
		customAssignedIds.emplace_back(id);
		return id;
	};

	static bool addSystem(ISystemBase& system) 
	{
		int size = systems.size();

		for (int i = 0; i < size; i++)
		{
			if (system.getSystemID() == systems.at(i).get().getSystemID())
			{
				return false;
			}
		}
		systems.emplace_back(system);
		return true;
	};

	static bool removeSystem(ISystemBase& system) 
	{ 
		int size = systems.size();

		for (int i = 0; i < size; i++)
		{
			if (system.getSystemID() == systems.at(i).get().getSystemID())
			{
				systems.erase(systems.begin() + i);
				return true;
			}
		}
		return false;
	}

	static void destroyEntity(int entityID, bool poolComponents = true)
	{
		int size = systems.size();

		for (int i = 0; i < size; i++)
		{
			if (poolComponents)
			{
				systems.at(i).get().removeComponents(entityID);
				continue;
			}
			systems.at(i).get().destroyComponents(entityID);
		}
	}


private:
	Entity();
	static int nextAssignableID;
	static std::vector<int> customAssignedIds;

	static std::vector<std::reference_wrapper<ISystemBase>> systems;

};

std::vector<std::reference_wrapper<ISystemBase>> Entity::systems = std::vector<std::reference_wrapper<ISystemBase>>();
