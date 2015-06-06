#include "World.h"

#include "Assert.h"


World::World()
	: mNextID(0)
{
}

World::~World()
{
	ForEachEntity( [&] (EntityID inID) { DestroyEntity( inID ); } );
}

EntityID World::CreateEntity()
{
	EntityID id = 0;

	if (mUnusedIDs.size() > 0)
	{
		EntityID out = mUnusedIDs.back();
		mUnusedIDs.pop_back();
		id = out;
	}
	else
	{
		id = mNextID++;
	}

	assert( !IsEntityAlive(id) );
	mUsedIDs.Set(id);

	return id;
}

bool World::IsEntityAlive(EntityID inID) const
{
	return mUsedIDs.Get(inID);
}

void World::DestroyEntity(EntityID inID)
{
	assert( IsEntityAlive(inID) );
	mComponentStorage.RemoveAllComponents(inID);
	mUnusedIDs.push_back(inID);
	mUsedIDs.Clear(inID);
}

bool World::HasComponent(EntityID inID, ComponentTypeID inTypeID) const
{
	return mComponentStorage.Has(inID, inTypeID);
}
