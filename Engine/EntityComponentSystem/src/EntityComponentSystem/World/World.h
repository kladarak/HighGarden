#pragma once

#include <vector>

#include <Core/Containers/BitField.h>

#include "ComponentTypeID.h"
#include "EntityTypedef.h"
#include "ComponentsStorage.h"

class World
{
public:
	World();
	~World();

	EntityID				CreateEntity();
	bool					IsEntityAlive(EntityID inID) const;
	void					DestroyEntity(EntityID inID);

	template<typename T, typename ...Args>
	void					AddComponent(EntityID inID, Args&&... inArgs);

	template<typename T>
	void					RemoveComponent(EntityID inID);

	template<typename T>
	bool					HasComponent(EntityID inID) const;
	bool					HasComponent(EntityID inID, ComponentTypeID inTypeID) const;

	template<typename T>
	T*						GetComponent(EntityID inID) const;

	template<typename TFunctor>
	void					ForEachEntity(const TFunctor& inFunctor) const;

private:
	EntityID				mNextID;
	std::vector<EntityID>	mUnusedIDs;
	BitField				mUsedIDs;

	ComponentsStorage		mComponentStorage;
};


template<typename T, typename ...Args>
void World::AddComponent(EntityID inID, Args&&... inArgs)
{
	mComponentStorage.Alloc<T>(inID, std::forward<Args>(inArgs)...);
}

template<typename T>
void World::RemoveComponent(EntityID inID)
{
	mComponentStorage.Dealloc<T>(inID);
}

template<typename T>
bool World::HasComponent(EntityID inID) const
{
	return HasComponent(inID, gGetComponentTypeID<T>());
}

template<typename T>
T* World::GetComponent(EntityID inID) const
{
	return mComponentStorage.Get<T>(inID);
}

template<typename TFunctor>
void World::ForEachEntity(const TFunctor& inFunctor) const
{
	for (size_t i = 0; i < mUsedIDs.NumBits(); ++i)
	{
		if (mUsedIDs.Get(i))
		{
			inFunctor( EntityID(i) );
		}
	}
}
