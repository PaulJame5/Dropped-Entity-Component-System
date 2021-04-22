/*
*	Apache 2.0 License https://opensource.org/licenses/Apache-2.0
* 	This is the single header include file for Dropped Entity Component System.
*	Created by Paul O'Callaghan as part of thesis looking at improving the Entity Component system by relegating
*	the Entity to an ID assigner type role instead of as a cotainer.
*/
#pragma once
#include <map>
#include <deque>
#include <vector>
#include <iostream>
#include <stdlib.h>     /* malloc, calloc, realloc, free */


namespace decs
{
	/// <summary>
	/// This is an inherited class to be used by all Components.
	/// </summary>
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
	/// <summary>
	/// Get the entity that the current component belongs to
	/// </summary>
	/// <returns></returns>
	inline int Component::belongsToID()
	{
		return belongsTo;
	}
	/// <summary>
	/// Set id that current component belongs to. Automatically assigned when called from ISystem::addComponent()
	/// </summary>
	/// <param name="id"></param>
	inline void Component::setBelongsToID(int id)
	{
		belongsTo = id;
	}
	/// <summary>
	/// Set whether component is active or not.
	/// </summary>
	inline void Component::setActive(bool active)
	{
		activeSelf = active;
	}
	/// <summary>
	/// Returns true if Component is active.
	/// </summary>
	/// <returns></returns>
	inline bool Component::isActive()
	{
		return activeSelf;
	}
} // End Component

// ISystemBase
namespace decs
{
	// Base class of ISystem. Used by Entity for when storing a deque of systems to check when deleting entities.
	class ISystemBase
	{
	public:
		ISystemBase();
		~ISystemBase();

		virtual void removeComponents(int entityId, bool immediate = false) = 0;

		virtual void destroyComponents(int entityId, bool immediate = false) = 0;

		virtual int getSystemID() = 0;
	};

	inline ISystemBase::ISystemBase() {}

	inline ISystemBase::~ISystemBase() {}

	// Storage of existing systems
	static int assignableSystemID = 0;
	static std::vector<std::reference_wrapper<ISystemBase>> systems = std::vector<std::reference_wrapper<ISystemBase>>();

	/// <summary>
	/// Used by ISystem automatically upon construction of an ISystem.
	/// Returns true if it is added successfully, false if it does not get added.
	/// </summary>
	/// <param name="system"> "The ISystem that you wish to have added to the deque."</param>
	/// <returns>description</returns>
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
	}

	/// <summary>
	/// Returns ID of a new entity then increments next assignable id. 
	/// This is called by ISystems on construction if they have no id assigned
	/// </summary>
	/// <returns>id of entity</returns>
	static int createNewSystemID()
	{
		return assignableSystemID++;
	}
} // End ISystemBase

// ISystem
namespace decs {
	/// <summary>
	/// class to be iherited by user defined Systems. ISystem has a deque of generic functions to be used with DECS.
	/// </summary>
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
		void removeComponent(int entityId, bool immediate = false);
		void removeComponents(int entityId, bool immediate = false);
		void removeComponentAt(int entityId, int position, bool immediate = false);

		// Destroys / Deletes component from memeory
		void destroyComponent(int entityId, bool immediate = false);
		void destroyComponents(int entityId, bool immediate = false);
		void destroyComponentAt(int entityId, int position, bool immediate = false);

		void cleanUpEntities();

		bool hasComponent(int entityId);

		// Returns first element null/blank if empty
		T& getComponent(int entityId);

		// Returns deque of components type T attached to entity
		std::vector<T>& getComponents(int entityId);


		std::map<int, std::vector<T>>& getEntities();

		void setPoolMaximumSize(int maximumSize);
		std::deque<T>& getRecycalblePool();
		int getPoolSize();
		void resetPool();

		int getSystemID();

	protected:
		static std::map<int, std::vector<T>> entities;
		static std::deque<T> recycablePool;

	private:
		static int systemID;
		static int maxPoolSize;
	};

	template<class T>
	inline ISystem<T>::ISystem()
	{
		if (systemID != -1)
		{
			return;
		}
		systemID = decs::createNewSystemID();
		decs::addSystem(*this);
	}

	template<class T>
	inline ISystem<T>::~ISystem() {}

	template<class T>
	std::map<int, std::vector<T>> ISystem<T>::entities = std::map<int, std::vector<T>>();

	template<class T>
	std::deque<T> ISystem<T>::recycablePool = std::deque<T>();

	template<class T>
	int ISystem<T>::maxPoolSize = 10000;

	template<class T>
	int ISystem<T>::systemID = -1;

	template<class T>
	inline int ISystem<T>::getSystemID()
	{
		return systemID;
	}

	/// <summary>
	/// Adds Component of type T with it's default Constructor to given Entity with ID.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="entityId"></param>
	template<class T>
	inline void ISystem<T>::addComponent(int entityId)
	{
		std::vector<T>& componentdeque = entities[entityId];
		if (recycablePool.empty() == false)
		{
			recycablePool.back().setBelongsToID(entityId);
			componentdeque.emplace_back(std::move(recycablePool.back()));
			recycablePool.pop_back();
			componentdeque.back().initialise();
			return;
		}

		componentdeque.emplace_back();
		componentdeque.back().setBelongsToID(entityId);
	}

	/// <summary>
	/// Adds component with values to an Entity with given ID.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="entityId"></param>
	template<class T>
	inline void ISystem<T>::addComponent(int entityId, T& newComponent)
	{
		newComponent.setBelongsToID(entityId);
		entities[entityId].emplace_back(newComponent);
	}

	/// <summary>
	///	Removes last component added to an entity and stores in a pool.
	/// Set Immediate to true if you don't want to check if the entity has a comoonent.
	/// </summary>
	template<class T>
	inline void ISystem<T>::removeComponent(int entityId, bool immediate)
	{
		if (!immediate)
		{
			if (!hasComponent(entityId))
			{
				return;
			}
		}

		// Cache optimisation
		std::vector<T>& componentdeque = entities[entityId];
		if (recycablePool.size() < maxPoolSize)
		{
			recycablePool.emplace_back(componentdeque.back());
		}
		componentdeque.pop_back();
	}

	/// <summary>
	/// Removes all components attached to an entity and moves themm to a pool.
	/// Set Immediate to true if you don't want to check if the entity has a comoonent.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="entityId"></param>
	template<class T>
	inline void ISystem<T>::removeComponents(int entityId, bool immediate)
	{
		if (!immediate)
		{
			if (!hasComponent(entityId))
			{
				return;
			}
		}

		// Cache optimisation
		std::vector<T>& componentdeque = entities[entityId];
		for (int i = componentdeque.size() - 1; i > -1; i--)
		{
			if (recycablePool.size() < maxPoolSize)
			{
				recycablePool.emplace_back(componentdeque.at(i));
				continue;
			}
			break;
		}
		componentdeque.clear();
	}

	/// <summary>
	/// Deletes element at position.
	/// Set Immediate to true if you don't want to check if the entity has a comoonent.
	/// </summary>
	/// <typeparam name="T">Must have a base class of Component</typeparam>
	/// <param name="entityId">Id of given entity</param>
	template<class T>
	inline void ISystem<T>::removeComponentAt(int entityId, int position, bool immediate)
	{
		if (!immediate)
		{
			if (!hasComponent(entityId))
			{
				return;
			}
		}
		// Cache optimisation
		std::vector<T>& componentdeque = entities[entityId];

		if (componentdeque.size() < position + 1)
		{
			return;
		}

		if (recycablePool.size() < maxPoolSize)
		{
			recycablePool.emplace_back(componentdeque.at(position));
		}

		componentdeque.erase(componentdeque.begin() + position);
	}

	/// <summary>
	///	Destroys last component added to an entity without adding it to the recycablePool deque.
	/// Set Immediate to true if you don't want to check if the entity has a comoonent.
	/// </summary>
	template<class T>
	inline void ISystem<T>::destroyComponent(int entityId, bool immediate)
	{
		if (!immediate)
		{
			if (!hasComponent(entityId))
			{
				return;
			}
		}
		// Cache optimisation
		std::vector<T>& componentdeque = entities[entityId];

		componentdeque.erase(componentdeque.end() - 1);

		if (componentdeque.size() == 0)
		{
			componentdeque.clear();
			componentdeque.shrink_to_fit();
		}
	}

	/// <summary>
	/// Deletes element at position. 
	/// Set Immediate to true if you don't want to check if the entity has a comoonent.
	/// </summary>
	/// <typeparam name="T">Must have a base class of Component</typeparam>
	/// <param name="entityId">Id of given entity</param>
	template<class T>
	inline void ISystem<T>::destroyComponentAt(int entityId, int position, bool immediate)
	{
		if (!immediate)
		{
			if (!hasComponent(entityId))
			{
				return;
			}
		}
		// Cache optimisation
		std::vector<T>& componentdeque = entities[entityId];

		// Check is valid position
		if (componentdeque.size() < position + 1)
		{
			return;
		}

		componentdeque.erase(componentdeque.begin() + position);
	}

	/// <summary>
	/// Deletes all components and removes entity from map without assigning objects to a pool.
	/// Set Immediate to true if you don't want to check if the entity has a comoonent.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="entityId"></param>
	/// <returns></returns>
	template<class T>
	inline void ISystem<T>::destroyComponents(int entityId, bool immediate)
	{
		if (!immediate)
		{
			if (!hasComponent(entityId))
			{
				return;
			}
		}
		entities[entityId].clear();
		
	}

	/// <summary>
	/// Deletes all components and removes entity from map without assigning objects to a pool 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="entityId"></param>
	/// <returns></returns>
	template<class T>
	inline void ISystem<T>::cleanUpEntities()
	{
		typename std::map<int, std::vector<T> >::iterator it = entities.begin();
		typename std::map<int, std::vector<T> >::iterator next = entities.begin();

		for (; it != entities.end(); )
		{
			if (it->second.size() == 0)
			{
				entities.erase(it++);
			}
			else
			{
				it++;
			}
		}
		if (entities.empty())
		{
			std::map<int, std::vector<T> > empty = std::map<int, std::vector<T> >();
			entities.swap(empty);
		}
	}

	/// <summary>
	/// Returns true if a component contains a component of type T
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="entityId"></param>
	/// <returns></returns>
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

	/// <summary>
	/// Returns the first component of type T attached to an entity.
	/// If no component is attached then a component will be added and then returned.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<class T>
	inline T& ISystem<T>::getComponent(int entityId)
	{
		if (!hasComponent(entityId))
		{
			addComponent(entityId);
		}

		return entities[entityId].at(0);
	}

	/// <summary>
	/// Returns a vector of all components attached to an entity. 
	/// If no components are added to an id then a component is added and then returned.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<class T>
	inline std::vector<T>& ISystem<T>::getComponents(int entityId)
	{
		if (!hasComponent(entityId))
		{
			addComponent(entityId);
		}

		return entities[entityId];
	}

	/// <summary>
	///	Returns current size of recycablePool
	/// </summary>
	template<class T>
	inline int ISystem<T>::getPoolSize()
	{
		return getRecycalblePool().size();
	}

	/// <summary>
	///	Sets Maximum Size of the Pool. Defaults at 10000.
	/// </summary>
	template<class T>
	inline void ISystem<T>::setPoolMaximumSize(int maximumSize)
	{
		maxPoolSize = maximumSize;
	}

	/// <summary>
	/// Returns reference to ISystem entites.
	/// Entities are stored in a map with the Key being an int to identify entites, 
	/// value is the components considered attached to an enitity. 
	/// </summary>
	template<class T>
	inline std::map<int, std::vector<T>>& ISystem<T>::getEntities()
	{
		return entities;
	}

	/// <summary>
	/// Returns reference to the recycablePool member variable.
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	template<class T>
	inline std::deque<T>& ISystem<T>::getRecycalblePool()
	{
		return recycablePool;
	}

	/// <summary>
	/// Resets pool of ISystem to size 0 and frees it from memory
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	template<class T>
	inline void ISystem<T>::resetPool()
	{
		recycablePool.resize(0);
		recycablePool.shrink_to_fit();
	}
} // End ISystem

// Dropped Entity fucntions
namespace decs
{
	// IDs for components
	static int nextAssignableID = 0;
	static std::vector<int> customAssignedIds = std::vector<int>();

	/// <summary>
	/// Returns ID of a new entity then increments next assignable id. Doesn't add containers this needs to be done by
	/// the user like so:
	/// Entity e = createNewEnitityID();
	/// ISystem::addComponent(e);
	/// </summary>
	/// <param name="immediate">Set to true if you don't want to check if a user defined id is being used by new id</param>
	/// <returns>id of entity</returns>
	static int createNewEnitityID(bool immediate)
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
	}

	/// <summary>
	/// This removes a System from Entity::systems vector. 
	/// Returns true if it is found and erased. False if it is now found.
	/// Systems are automatically added to the deque on construction. 
	/// </summary>
	/// <param name="system"> "The ISystem that you wish to have removed from the deque."</param>
	/// <returns>int of newly created Entity</returns>
	static int createNewEnitityWithID(int id, bool immediate)
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
	}

	/// <summary>
	/// This removes a System from Entity::systems vector. 
	/// Returns true if it is found and erased. False if it is now found.
	/// Systems are automatically added to the deque on construction. 
	/// </summary>
	/// <param name="system"> "The ISystem that you wish to have removed from the deque."</param>
	/// <returns>true on success, false on failure</returns>
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

	/// <summary>
	/// Calls ISystem::removeComponents on the given entity ID by default. Set to false to call
	/// ISystem::destroyComponents.
	/// </summary>
	/// <param name="entityID">ID of entity that is being destroyed.</param>
	/// <returns></returns>
	/// <param name="poolComponents">True by default, set to false to call the destructor instead of pooling.</param>
	/// <returns></returns>
	static void destroyEntity(int entityID,bool skipCustomIDCheck = false ,bool poolComponents = true)
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

		// check customAssignableID to free it for reuse
		if (skipCustomIDCheck)
		{
			return;
		}

		size = customAssignedIds.size();
		for (int i = 0; i < size; i++)
		{
			if (customAssignedIds.at(i) == entityID)
			{
				customAssignedIds.erase(customAssignedIds.begin() + i);
				return;
			}
		}
	}
} // End Dropped Entity Functions
