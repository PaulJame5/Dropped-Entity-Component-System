/*
*	Apache 2.0 License https://opensource.org/licenses/Apache-2.0
* 	This is the single header include file for Dropped Entity Component System.
*
*	Created by Paul O'Callaghan as part of thesis looking at improving the Entity Component
*	system by removing the need for entities.
*
*	SparseSet is a modified class based on Sam Griffiths class template of a sparse set of integers
*	found here: https://gist.github.com/sjgriffiths/06732c6076b9db8a7cf4dfe3a7aed43a
*/

#pragma once
#include <iostream>
#include <algorithm>
#include <deque>
#include <vector>
#include <type_traits>

namespace decs
{
	class Component;

	template <class T>
	class SparseSet
	{
		static_assert(std::is_convertible<T*, Component*>::value, "class<T> Must inherit from Component");

	private:
		static int size_dense_vector;
		static int capacity_sparse_vector;

	protected:
		static std::vector<T> dense;
		static std::vector<std::vector<int>> sparse;

		/// <summary>
		/// Method to perform a default insert of a newly constructed component.
		/// </summary>
		/// <param name="id">ID to tag component with.</param>
		void defaultInsert(int id);

		/// <summary>
		/// Method to perform a pooled insert.
		/// </summary>
		/// <param name="id">ID to tag component with.</param>
		void pooledInsert(int id);

		/// <summary>
		/// Remove algorithm that handles organising dense list for pooling and destrtuction.
		/// Will reorder components if a component is moved closer to the beginning of dense list.
		/// Best case O(1), worst case O(N) based on complexity of copy constructor.
		/// </summary>
		/// <param name="id">ID of component to perform removal/deletion on.</param>
		/// <param name="index">Index position of component to perform removal/deletion on.</param>
		void rem(int id, int index = 0);

	public:
		SparseSet();
		~SparseSet();


		/// <summary>
		/// Returns begin iterator of dense list.
		/// </summary>
		/// <returns>Returns begin iterator of dense list.</returns>
		typename std::vector<T>::iterator begin();

		/// <summary>
		/// Retruns last element iterator of dense list.
		/// </summary>
		/// <returns>Retruns last element iterator of dense list.</returns>
		typename std::vector<T>::iterator back();

		/// <summary>
		/// Iterator end of dense list.
		/// </summary>
		/// <returns>End iterator of dense list.</returns>
		typename std::vector<T>::iterator end();

		/// <summary>
		/// Returns size of used components in dense list.
		/// </summary>
		/// <returns>Returns size of used components in dense list.</returns>
		int size();

		/// <summary>
		/// Returns the amount of IDs in use. Highest id that can be used would be numberOfIDs - 1.
		/// </summary>
		/// <returns>Returns the amount of IDs in use.</returns>
		int numberOfIDs();

		/// <summary>
		/// Returns size of dense list both active and inactive.
		/// </summary>
		/// <returns>Returns size of dense list both active and inactive.</returns>
		int denseListSize();

		/// <summary>
		/// Cheks if dense list is currently empty.
		/// </summary>
		/// <returns>True if dense list contains no active objects, otherwise returns false if contains objects.</returns>
		bool empty();

		/// <summary>
		/// Clears dense list, sparse list and resizes everything back to 0.
		/// </summary>
		void clear();

		/// <summary>
		/// Calls reser on sparse list. Effectively tells the array the highest id that will be used/is in use.
		/// </summary>
		/// <param name="u"></param>
		void reserveIDCapacity(int u);

		/// <summary>
		/// Calls reserve on dense list.
		/// </summary>
		/// <param name="u">Number of components size to reserve.</param>
		void reserveComponentCapacity(int u);

		/// <summary>
		/// Checks if component with id exists. Returs false if is doesn't otherwise it returns true.
		/// </summary>
		/// <param name="id">ID of component to check if it exists.</param>
		/// <returns>True if component has ID, false if not found.</returns>
		bool has(const int id);

		/// <summary>
		/// Inserts component with ID to dense list. If there is a pooled object this will be initialised with the
		/// values instead of constructing a new one.
		/// </summary>
		/// <param name="id">ID to tag component with.</param>
		void insert(const int id);

		/// <summary>
		/// Removes all objects not used in the dense list effectively making dense.size() = dense_vector_size.
		/// </summary>
		void removePooledObjects();

		/// <summary>
		/// For adding another component to self. Only use if you are sure there's another component already attached
		/// as no checks are performed when using this method.
		/// </summary>
		/// <param name="id">ID to tag component with.</param>
		void insertAnotherToSelf(int id);

		/// <summary>
		/// Returns pointer to first component found with id. Returns nullptr if not found. If dense list resizes after
		/// lookup pointer will become invalidated.
		/// </summary>
		/// <param name="id">ID of component to be found.</param>
		/// <returns>Pointer to component.</returns>
		T* ptrGet(const int id);

		/// <summary>
		/// Returns pointer to component in dense list at given index relative to id.
		/// Returns nullptr if not found. If dense list is resized after lookup pointer will be invalidated.
		/// </summary>
		/// <param name="id">ID of component to return.</param>
		/// <param name="index">Index of component relative to id.</param>
		/// <returns>Pointer to component.</returns>
		T* ptrGetAtIndex(const int id, const int index);

		/// <summary>
		/// Retruns reference to first found component with given id in the dense list.
		/// Results in undefined behaviour if component doesn't exist. Use with caution.
		/// </summary>
		/// <param name="id">ID of component to be returned.</param>
		/// <returns>Reference to component in dense list</returns>
		T& get(const int id);

		/// <summary>
		/// Returns reference to component with id at index position. Results in undefined behaviour if component doesn't
		/// exists. Use with caution.
		/// </summary>
		/// <param name="id">ID of component to be returned.</param>
		/// <param name="index">Index of component relative to id to be returned.</param>
		/// <returns>Reference to Component in dense list.</returns>
		T& getAtIndex(const int id, const int index);

		/// <summary>
		/// Returns reference to the dense list of components.
		/// </summary>
		/// <returns>Dense list of components</returns>
		std::vector<T>& getDenseList();

		/// <summary>
		/// Emplaces component to end of dense list with given id.
		/// </summary>
		/// <param name="id">ID of component to be emplaced</param>
		/// <param name="emplaced">Component to have it's values emplaced.</param>
		void emplace(const int id, T&& emplaced);

		/// <summary>
		/// Inserts a copy of components values to the end of dense list with given id.
		/// </summary>
		/// <param name="id">ID of component that notifies sparse.</param>
		/// <param name="copy">Component to have it's values copied.</param>
		void insertCopy(const int id, T& copy);

		/// <summary>
		/// Destroys first component found with given id that is closest to the begininng of dense list.
		/// </summary>
		/// <param name="id">ID tag of component to be destroyed.</param>
		/// <returns>True if component is destroyed, false if nothing is destroyed.</returns>
		bool eraseWithID(const int id);

		/// <summary>
		/// Destroys all component with a given id.
		/// </summary>
		/// <param name="id">ID of components to be destroyed.</param>
		/// <returns>True if components are destroyed, false if nothing is destroyed.</returns>
		bool eraseAllWithID(const int id);

		/// <summary>
		/// Destroys component with id tag at the given index. If component can't be found this
		/// does nothing.
		/// </summary>
		/// <param name="id"></param>
		/// <param name="index"></param>
		/// <returns></returns>
		bool eraseWithIDAtIndex(const int id, int index);

		/// <summary>
		/// Removes first component found closest to the beginning of the dense list. Removed component
		/// is placed into a pool.
		/// </summary>
		/// <param name="id">ID tag of component to be removed.</param>
		/// <returns>True if component removed, false if nothing is removed.</returns>
		bool removeWithID(const int id);

		/// <summary>
		/// Removes all components with given id. Removed components are moved to a pool.
		/// </summary>
		/// <param name="id">ID tag of components you want removed.</param>
		/// <returns>True if all components removed, false if none are removed.</returns>
		bool removeAllWithID(const int id);

		/// <summary>
		/// Removes all components with given id at index based on position closest
		/// to dense list begin. Removed components are moved to a pool.
		/// If index isn't found this will return false.
		/// </summary>
		/// <param name="id">ID tag of component to be removed.</param>
		/// <param name="index">Index of component to be removed.</param>
		/// <returns>True if component removed, false if none are removed.</returns>
		bool removeWithIDAtIndex(const int id, const int index);

		/// <summary>
		/// Returns the number of components that share the given id.
		/// </summary>
		/// <param name="id">ID of components you want to check.</param>
		/// <returns>Number of components with given id.</returns>
		int numberOfComponentsWithID(const int id);

		/// <summary>
		/// Returns the number of components that are able to run update in dense list.
		/// </summary>
		/// <returns>Number of components that can update in dense list.</returns>
		int getNumberOfActiveComponents();

		/// <summary>
		/// Runs update on every active component in dense list.
		/// </summary>
		void runUpdate();

		/// <summary>
		/// replaces component values with id that is closest to the start of the dense list.
		/// </summary>
		/// <param name="id">ID tag of the component</param>
		/// <param name="copy">Component values you want copied</param>
		void replace(const int id, T& copy);

		/// <summary>
		/// Replace a component with anothers value. If there is no component this does nothing.
		/// </summary>
		/// <param name="id">ID tag of component</param>
		/// <param name="componentPosition">whether you want the 1st, 2nd... component</param>
		/// <param name="copy">Component values you want copied</param>
		void replace(const int id, int componentPosition, T& copy);

		/// <summary>
		/// For debugging purposes, print out all elements containing components
		/// </summary>
		void print();
	};

	template <class T>
	std::vector<T> SparseSet<T>::dense = std::vector<T>();	//Dense set of elements

	template <class T>
	std::vector<std::vector<int>> SparseSet<T>::sparse(0, std::vector<int>(0));	//Map of elements to dense set indices

	template <class T>
	int SparseSet<T>::size_dense_vector = 0;	// Current size (number of elements)

	template <class T>
	int SparseSet<T>::capacity_sparse_vector = 0;	//Current capacity (maximum value + 1)

	template<class T>
	inline SparseSet<T>::SparseSet()
	{
		dense.clear();
		sparse.clear();
	}

	template<class T>
	inline SparseSet<T>::~SparseSet()
	{

	}

	template<class T>
	inline typename std::vector<T>::iterator SparseSet<T>::begin()
	{
		return dense.begin();
	}

	template<class T>
	inline typename std::vector<T>::iterator SparseSet<T>::back()
	{
		return dense.begin() + (size_dense_vector - 1);
	}

	template<class T>
	inline typename std::vector<T>::iterator SparseSet<T>::end()
	{
		return dense.begin() + size_dense_vector;
	}

	template<class T>
	inline int SparseSet<T>::size()
	{
		return size_dense_vector;
	}

	template<class T>
	inline int SparseSet<T>::numberOfIDs()
	{
		return capacity_sparse_vector;
	}

	template<class T>
	inline int SparseSet<T>::denseListSize()
	{
		return dense.size();
	}

	template<class T>
	inline bool SparseSet<T>::empty()
	{
		return size_dense_vector == 0;
	}

	template<class T>
	inline void SparseSet<T>::clear()
	{
		dense.clear();
		dense.resize(0);
		size_dense_vector = 0;
		sparse.clear();
		sparse.resize(0);
		capacity_sparse_vector = 0;
	}

	template<class T>
	inline void SparseSet<T>::reserveIDCapacity(int u)
	{
		if (u > capacity_sparse_vector)
		{
			sparse.resize(u);
			capacity_sparse_vector = u;
		}
	}

	template<class T>
	inline void SparseSet<T>::reserveComponentCapacity(int u)
	{
		if (u > dense.size())
		{
			dense.reserve(u);
		}
	}

	template<class T>
	inline bool SparseSet<T>::has(const int id)
	{
		if (id < 0)
		{
			return false;
		}

		if (id >= capacity_sparse_vector)
		{
			return false;
		}

		if (size_dense_vector == 0)
		{
			return false;
		}
		std::vector<int>& sparseCache = sparse[id];

		if (sparseCache.empty())
		{
			return false;
		}

		return true;
	} // end has(id)

	template<class T>
	inline void SparseSet<T>::insert(const int id)
	{
		if (id < 0)
		{
			return;
		}
		if (id >= capacity_sparse_vector)
		{
			reserveIDCapacity(id + 1);
		}

		if (dense.size() <= size_dense_vector)
		{
			defaultInsert(id);
			return;
		}
		pooledInsert(id);
	}

	template<class T>
	inline void SparseSet<T>::defaultInsert(int id)
	{
		dense.emplace_back();
		T& created = dense[size_dense_vector];
		created.setBelongsToID(id);
		created.setActive(true);
		created.initialise();

		sparse.at(id).push_back(size_dense_vector);
		++size_dense_vector;
	}

	template<class T>
	inline void SparseSet<T>::pooledInsert(int id)
	{
		T& recycaled = dense[size_dense_vector];
		recycaled.setBelongsToID(id);
		recycaled.setActive(true);
		recycaled.initialise();

		sparse.at(id).push_back(size_dense_vector);
		++size_dense_vector;
	}

	template<class T>
	inline void SparseSet<T>::removePooledObjects()
	{
		while (dense.size() > size_dense_vector)
		{
			dense.pop_back();
		}
	}

	template<class T>
	inline void SparseSet<T>::insertAnotherToSelf(int id)
	{
		defaultInsert(id);
	}

	template<class T>
	inline T* SparseSet<T>::ptrGet(const int id)
	{
		if (!has(id))
		{
			return nullptr;
		}
		return &dense[sparse.at(id).at(0)];
	}

	template<class T>
	inline T* SparseSet<T>::ptrGetAtIndex(const int id, const int index)
	{
		if (!has(id))
		{
			return nullptr;
		}
		if (sparse.at(id).size() <= index)
		{
			return nullptr;
		}
		return &dense[sparse.at(id).at(index)];
	}

	template<class T>
	inline T& SparseSet<T>::get(const int id)
	{
		return dense[sparse.at(id).at(0)];
	}

	template<class T>
	inline T& SparseSet<T>::getAtIndex(const int id, const int index)
	{
		return dense[sparse.at(id).at(index)];
	}

	template<class T>
	inline std::vector<T>& SparseSet<T>::getDenseList()
	{
		return dense;
	}

	template<class T>
	inline void SparseSet<T>::emplace(const int id, T&& emplaced)
	{
		if (id < 0)
		{
			return;
		}

		if (id >= capacity_sparse_vector)
		{
			reserveIDCapacity(id + 1);
		}

		dense.emplace_back(emplaced);
		dense[size_dense_vector].setBelongsToID(id);
		sparse.at(id).push_back(size_dense_vector);
		++size_dense_vector;
	}

	template<class T>
	inline void SparseSet<T>::insertCopy(const int id, T& copy)
	{
		if (id < 0)
		{
			return;
		}

		if (id >= capacity_sparse_vector)
		{
			reserveIDCapacity(id + 1);
		}

		dense.push_back(copy);
		dense[size_dense_vector].setBelongsToID(id);
		sparse.at(id).push_back(size_dense_vector);
		++size_dense_vector;
	}

	template<class T>
	inline bool SparseSet<T>::eraseWithID(const int id)
	{
		if (!removeWithID(id))
		{
			return false;
		}
		dense.pop_back();
		return true;
	}

	template<class T>
	inline bool SparseSet<T>::eraseAllWithID(const int id)
	{
		if (!has(id))
		{
			return false;
		}
		while (sparse.at(id).size() > 0)
		{
			rem(id);
			dense.pop_back();
		}
		return true;
	}

	template<class T>
	inline bool SparseSet<T>::eraseWithIDAtIndex(const int id, int index)
	{
		if (!removeWithIDAtIndex(id, index))
		{
			return false;
		}
		dense.pop_back();
		return true;
	}

	template<class T>
	inline bool SparseSet<T>::removeWithID(const int id)
	{
		if (!has(id))
		{
			return false;
		}
		rem(id);
		return true;
	}

	template<class T>
	inline bool SparseSet<T>::removeAllWithID(const int id)
	{
		if (!has(id))
		{
			return false;
		}
		while (!sparse.at(id).empty())
		{
			rem(id);
		}
		return true;
	}

	template<class T>
	inline bool SparseSet<T>::removeWithIDAtIndex(const int id, const int index)
	{
		if (!has(id))
		{
			return false;
		}
		if (sparse[id].size() <= index)
		{
			return false;
		}
		rem(id);
		return true;
	}

	template<class T>
	inline int SparseSet<T>::numberOfComponentsWithID(const int id)
	{
		if (!has(id))
		{
			return 0;
		}
		return sparse.at(id).size();
	}

	template<class T>
	inline int SparseSet<T>::getNumberOfActiveComponents()
	{
		return size_dense_vector;
	}

	template<class T>
	inline void SparseSet<T>::runUpdate()
	{
		for (int i = 0; i < size_dense_vector; i++)
		{
			if (!dense[i].isActive())
			{
				continue;
			}
			dense.at(i).update();
		}
	}

	template<class T>
	inline void SparseSet<T>::replace(const int id, T& copy)
	{
		if (!has(id))
		{
			return;
		}
		copy.setBelongsToID(id);
		dense.at(sparse.at(id).at(0)) = copy;
	}

	template<class T>
	inline void SparseSet<T>::replace(const int id, int componentPosition, T& copy)
	{
		if (!has(id))
		{
			return;
		}
		if (componentPosition >= sparse.at(id).size())
		{
			return;
		}
		copy.setBelongsToID(id);
		dense.at(sparse.at(id).at(componentPosition)) = copy;
	}

	template<class T>
	inline void SparseSet<T>::print()
	{
		if (size_dense_vector == 0)
		{
			printf("no components in dense array");
			printf("\n");
			return;
		}
		for (int i = 0; i < size_dense_vector; i++)
		{
			printf("%d, ", dense[i].belongsToID());
		}
		printf("\n");
	}

	template<class T>
	inline void SparseSet<T>::rem(int id, int index)
	{
		int removedComponentPosition = sparse.at(id).at(index);

		if (removedComponentPosition == size_dense_vector - 1)
		{
			sparse.at(id).erase(sparse.at(id).begin() + index);
			--size_dense_vector;
			return;
		}

		dense[removedComponentPosition].setActive(false);

		int lastElementBelongID = dense[size_dense_vector - 1].belongsToID();

		sparse[lastElementBelongID].back() = removedComponentPosition;
		dense[removedComponentPosition] = dense[size_dense_vector - 1];

		if (sparse[lastElementBelongID].size() > 1)
		{
			std::sort(sparse[lastElementBelongID].begin(), sparse[lastElementBelongID].end());
		}

		sparse.at(id).erase(sparse.at(id).begin() + index);
		--size_dense_vector;
	} // End rem(id);

} // End sparse


// System base

// SystemBase
namespace decs
{
	// Base class of System. Used by Entity for when storing 
	// a deque of systems to check when deleting entities.
	// Do not construct or use this by itself as it will do nothing.
	class SystemBase
	{
	public:
		SystemBase();

		~SystemBase();

		virtual bool removeAllComponentsWithID(int entityId) = 0;

		virtual bool destroyAllComponentsWithID(int entityId) = 0;

		virtual bool hasComponentWithID(int entityID) = 0;

		virtual int getSystemID() = 0;

		virtual void update() = 0;

		virtual int highestIDUsed() = 0;

		virtual int getNumberOfActiveComponents() = 0;

		virtual void clear() = 0;
	};

	inline SystemBase::SystemBase() {}

	inline SystemBase::~SystemBase() {}

} // End SystemBase

namespace decs
{
	class World
	{
	private:
		// Storage of existing systems
		static int assignableSystemID;
		static std::vector<std::reference_wrapper<SystemBase>> systems;

		static int nextAvailableID;
		static std::deque<int> reusableIds;

		static std::deque<int> destroyListPool;
		static std::deque<int> destroyList;

		static float deltaTime;

	public:
		/// <summary>
		/// Used by System automatically upon construction of an System.
		/// Returns true if it is added successfully, 
		/// false if it does not get added.
		/// </summary>
		/// <param name="system"> "The System that you wish 
		/// to have added to the deque."</param>
		/// <returns>description</returns>
		static bool addSystem(SystemBase& system);

		/// <summary>
		/// Set new delta time. Do this at the begininng of every frame to make sure your programs are frame rate
		/// independant;
		/// </summary>
		/// <param name="dt">delta time float to be passed</param>
		static void setDeltaTime(float dt);

		/// <summary>
		/// Get delta time of current frame
		/// </summary>
		static float getDeltaTime();
		/// <summary>
		/// Returns ID to be used. Will use one from reusable 
		/// ids if one is available.
		/// </summary>
		/// <returns>Id for use.</returns>
		static int createNewID();

		/// <summary>
		/// Returns ID of a new entity then increments next assignable id. 
		/// This is called by Systems on construction if 
		/// they have no id assigned to them yet.
		/// </summary>
		/// <returns>id of entity</returns>
		static int createNewSystemID();

		/// <summary>
		/// Calls update on all systems that have 
		/// set allowUpdate to true. 
		/// </summary>
		static void update();

		/// <summary>
		/// Marks entity to be destroyed. 
		/// If using built in update these entites are removed 
		/// or destroyed at the end of the update cycle.
		/// </summary>
		/// <param name="entityID">ID of components to be removed.</param>
		/// <param name="poolComponents">Whether to store 
		/// components in a pool or not. True by default.</param>
		static void destroyEntity(int entityID, bool poolComponents = true);

		/// <summary>
		/// Marks all ids and components for destruction. 
		/// If using built in update components will be destroyed at 
		/// the end of the update cycle.
		/// </summary>
		/// <param name="poolComponents">Whether to store 
		/// components for reuse or not. Set to false by default.</param>
		static void destroyAllEntities(bool poolComponents = false);

		/// <summary>
		/// Finds all ids used that have no components and 
		/// places them into the id pool to be reused.
		/// </summary>
		static void destroyOrphanedEntities();

		/// <summary>
		/// Destroys marked components. This is called automatically 
		/// with the built in update.
		/// 
		/// When using custom updates this needs to be 
		/// called by the user.
		/// </summary>
		static void destroyMarked();

		/// <summary>
		/// Returns what the next availble id will be but doesn't increment. 
		/// Instead use World::createNewId() to assign a new id as this will use a pool and
		/// auto increment to the next available.
		/// </summary>
		/// <returns>an int that the next available id will be not including those in the pool</returns>
		static int getNextAvailableEntityID();
	};

	inline void World::setDeltaTime(float dt)
	{
		deltaTime = dt;
	}

	inline float World::getDeltaTime()
	{
		return deltaTime;
	}

	inline int World::createNewID()
	{
		if (reusableIds.empty())
		{
			return nextAvailableID++;
		}
		int returnedID = reusableIds.back();
		reusableIds.pop_back();
		return returnedID;
	}

	inline bool World::addSystem(SystemBase& system)
	{
		size_t size = systems.size();

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

	inline int World::createNewSystemID()
	{
		return assignableSystemID++;
	}

	inline void World::update()
	{
		size_t size = systems.size();
		for (int i = 0; i < size; i++)
		{
			systems.at(i).get().update();
		}
		// Clean up components marked for destruction.
		destroyMarked();
	}

	inline void World::destroyEntity(int entityID, bool poolComponents)
	{
		size_t size = systems.size();

		for (int i = 0; i < size; i++)
		{
			if (systems.at(i).get().hasComponentWithID(entityID))
			{
				if (poolComponents)
				{
					destroyListPool.push_back(entityID);
					break;
				}
				destroyList.push_back(entityID);
				break;
			}
		}
	}

	inline void World::destroyAllEntities(bool poolComponents)
	{
		size_t systemSize = systems.size();
		int highestID = 0;

		for (int i = 0; i < systemSize; i++)
		{
			int highestSystemID = systems.at(i).get().highestIDUsed();
			if (highestSystemID > highestID)
			{
				highestID = highestSystemID;
			}
		}

		for (int j = 0; j <= highestID; j++)
		{
			destroyEntity(j, poolComponents);
		}
	}

	inline void World::destroyOrphanedEntities()
	{
		size_t systemSize = systems.size();
		int highestID = 0;

		for (int i = 0; i < systemSize; i++)
		{
			int highestSystemID = systems.at(i).get().highestIDUsed();

			if (highestSystemID > highestID)
			{
				highestID = highestSystemID;
			}
		}

		std::deque<int> destroyList;
		bool destroyable = false;

		for (int i = 0; i <= highestID; i++)
		{
			for (int systemID = 0; systemID < systemSize; systemID++)
			{
				if (!systems.at(systemID).get().hasComponentWithID(i))
				{
					destroyList.push_back(i);
					break;
				}
			}
		}

		while (destroyList.empty() == false)
		{
			reusableIds.push_back(destroyList.back());
			destroyList.pop_back();
		}
	} // End destroyOrphanedentitiies();

	inline void World::destroyMarked()
	{
		while (destroyList.empty() == false)
		{
			for (int i = 0; i < systems.size(); i++)
			{
				systems.at(i).get().destroyAllComponentsWithID(destroyList.back());
			}
			reusableIds.push_back(destroyList.back());
			destroyList.pop_back();
		}
		while (destroyListPool.empty() == false)
		{
			for (int i = 0; i < systems.size(); i++)
			{
				systems.at(i).get().removeAllComponentsWithID(destroyListPool.back());
			}
			reusableIds.push_back(destroyListPool.back());
			destroyListPool.pop_back();
		}
	} // end Destroy();

	inline int World::getNextAvailableEntityID()
	{
		return nextAvailableID;
	}

	int World::assignableSystemID = 0;
	std::vector<std::reference_wrapper<SystemBase>> World::systems = std::vector<std::reference_wrapper<SystemBase>>();
	std::deque<int> World::reusableIds = std::deque<int>();
	std::deque<int> World::destroyListPool = std::deque<int>();
	std::deque<int> World::destroyList = std::deque<int>();
	int World::nextAvailableID = 0;
	float World::deltaTime = 0;
}

namespace decs
{
	template<class T>
	class System : SystemBase
	{
		static_assert(std::is_convertible<T*, Component*>::value, "class<T> Must inherit from Component");
	public:
		System();

		void addComponentWithID(int id);

		void addComponentValuesWithID(int id, T& copy);

		void emplaceComponentWithID(int id, T&& emplaced);

		void removeComponentWithID(int id);

		bool removeAllComponentsWithID(int id) override;

		void destroyComponentWithID(int id);

		bool destroyAllComponentsWithID(int id) override;

		void replaceComponentWithID(int id, T& replacement);

		void replaceComponentWithIDAtIndex(int id, int index, T& replacement);

		void clear() override;

		bool hasComponentWithID(int id) override;

		T* getPtrComponentWithID(int id);

		T* getPtrComponentWithIDAtIndex(int id, int index);

		T& getComponentWithID(int id);

		T& getComponentWithIDAtIndex(int id, int index);

		int componentsSize();

		void reserveIDCapacity(int u);

		void reserveComponentCapacity(int u);

		int getNumberOfComponentsWithID(int id);

		int getNumberOfActiveComponents();

		int getSystemID() override;

		std::vector<T>& getDenseList();

		void update() override;

		void setCanUpdate(bool allow);

	protected:
		static SparseSet<T> entityManager;

	private:
		static int systemID;
		static bool allowUpdate;

		int highestIDUsed() override;

	};

	template<class T>
	SparseSet<T> System<T>::entityManager = SparseSet<T>();

	template<class T>
	int System<T>::systemID = -1;

	template<class T>
	bool System<T>::allowUpdate = true;

	template<class T>
	System<T>::System()
	{
		if (systemID != -1)
		{
			return;
		}
		systemID = decs::World::createNewSystemID();
		decs::World::addSystem(*this);

	}

	template<class T>
	void System<T>::update()
	{
		if (!allowUpdate)
		{
			return;
		}
		entityManager.runUpdate();
	}

	template<class T>
	int System<T>::highestIDUsed()
	{
		return entityManager.numberOfIDs();
	}

	template<class T>
	void System<T>::setCanUpdate(bool allow)
	{
		allowUpdate = allow;;
	}

	template<class T>
	void System<T>::addComponentWithID(int id)
	{
		entityManager.insert(id);
	}

	template<class T>
	inline void System<T>::addComponentValuesWithID(int id, T& copy)
	{
		entityManager.insertCopy(id, copy);
	}

	template<class T>
	inline void System<T>::emplaceComponentWithID(int id, T&& emplaced)
	{
		entityManager.emplace(id, emplaced);
	}

	template<class T>
	void System<T>::removeComponentWithID(int id)
	{
		entityManager.removeWithID(id);
	}

	template<class T>
	bool System<T>::removeAllComponentsWithID(int id)
	{
		return entityManager.removeAllWithID(id);
	}

	template<class T>
	void System<T>::destroyComponentWithID(int id)
	{
		entityManager.eraseWithID(id);
	}

	template<class T>
	bool System<T>::destroyAllComponentsWithID(int id)
	{
		return entityManager.eraseAllWithID(id);
	}

	template<class T>
	inline void System<T>::replaceComponentWithID(int id, T& replacement)
	{
		entityManager.replace(id, replacement);
	}

	template<class T>
	inline void System<T>::replaceComponentWithIDAtIndex(int id, int index, T& replacement)
	{
		entityManager.replace(id, index, replacement);
	}

	template<class T>
	inline void System<T>::clear()
	{
		entityManager.clear();
	}

	template<class T>
	bool System<T>::hasComponentWithID(int id)
	{
		return entityManager.has(id);
	}

	template<class T>
	T* System<T>::getPtrComponentWithID(int id)
	{
		return entityManager.ptrGet(id);
	}

	template<class T>
	T* System<T>::getPtrComponentWithIDAtIndex(int id, int index)
	{
		return entityManager.ptrGetAtIndex(id, index);
	}

	template<class T>
	T& System<T>::getComponentWithID(int id)
	{
		return entityManager.get(id);
	}

	template<class T>
	T& System<T>::getComponentWithIDAtIndex(int id, int index)
	{
		return entityManager.getAtIndex(id, index);
	}

	template<class T>
	std::vector<T>& System<T>::getDenseList()
	{
		return entityManager.getDenseList();
	}

	template<class T>
	int System<T>::componentsSize()
	{
		return entityManager.size();
	}

	template<class T>
	inline void System<T>::reserveIDCapacity(int u)
	{
		entityManager.reserveIDCapacity(u);
	}

	template<class T>
	inline void System<T>::reserveComponentCapacity(int u)
	{
		entityManager.reserveComponentCapacity(u);
	}

	template<class T>
	int System<T>::getNumberOfActiveComponents()
	{
		return entityManager.getNumberOfActiveComponents();
	}

	template<class T>
	int System<T>::getNumberOfComponentsWithID(int id)
	{
		return entityManager.numberOfComponentsWithID(id);
	}

	template<class T>
	int System<T>::getSystemID()
	{
		return systemID;
	}
} // End System<T>

namespace decs
{
	/// <summary>
	/// This is an inherited class to be used by all Components.
	/// </summary>
	class Component
	{
	public:
		Component();
		Component(const Component& c);

		virtual ~Component();

		/// <summary>
		/// Called on construction of new component or when reused 
		/// from the pool. 
		/// Default implementation does nothing.
		/// </summary>
		virtual void initialise();

		/// <summary>
		/// Get the entity that the current component belongs to
		/// </summary>
		/// <returns>ID tag of component</returns>
		int belongsToID();

		/// <summary>
		/// Set the Id tag of component. This is called automatically 
		/// when a component is constructed or reused from
		/// the pool
		/// </summary>
		/// <param name="id">ID tag of component.</param>
		void setBelongsToID(int id);

		/// <summary>
		/// Set whether to component is currently active. If this is 
		/// set to false the buil in update will skip it on 
		/// each update cycle until it is turned back on again.
		/// 
		/// Is automatically set to true on construction or when 
		/// recycled from a pool.
		/// </summary>
		/// <param name="active">Set whether component is active 
		/// or not.</param>
		void setActive(bool active);

		/// <summary>
		/// Returns whether the component is active or not.
		/// </summary>
		/// <returns>Returns whether the component is active 
		/// or not.</returns>
		bool isActive();

		/// <summary>
		/// Update method of component. Can be overrided for 
		/// custom logic. 
		/// Does nothing otherwise. 
		/// </summary>
		virtual void update();
	protected:
		int belongsTo = -1;
		bool activeSelf = true;
	};

	inline Component::Component()
	{

	}

	inline Component::Component(const Component& c)
	{
		activeSelf = c.activeSelf;
		belongsTo = c.belongsTo;
	}

	inline Component::~Component()
	{

	}

	inline void Component::initialise()
	{

	}

	inline int Component::belongsToID()
	{
		return belongsTo;
	}

	inline void Component::update()
	{
		// Does nothing if not implemented
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
} // End Component
