#include "World.h"

#include "Assert.h"
#include "EntityFilter.h"

bool Entity::IsAlive() const
{
	return mWorld->IsEntityAlive(mID);
}

void Entity::Kill()
{
	mWorld->DestroyEntity(mID);
}
	
bool Entity::HasComponent(ComponentTypeID inTypeID) const
{
	return mWorld->HasComponent(mID, inTypeID);
}

//-----------------------------------------------------------------

World::World()
	: mNextID(0)
{
}

World::~World()
{
	ForEachEntity( [&] (Entity inEntity) { DestroyEntity( inEntity.GetID() ); } );
}

Entity World::CreateEntity()
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

	return Entity(id, this);
}

bool World::IsEntityAlive(EntityID inID) const
{
	return mUsedIDs.Get(inID);
}

void World::DestroyEntity(EntityID inID)
{
	assert( IsEntityAlive(inID) );
	mUsedIDs.Clear(inID);
	mQueuedIDsForDestruction.push_back(inID);
}

void World::FlushDestroyedEntities()
{
	for (EntityID id : mQueuedIDsForDestruction)
	{
		mComponentStorage.RemoveAllComponents(id);
		mUnusedIDs.push_back(id);
	}

	mQueuedIDsForDestruction.clear();
}

bool World::HasComponent(EntityID inID, ComponentTypeID inTypeID) const
{
	return mComponentStorage.Has(inID, inTypeID);
}

std::vector<Entity> World::GetEntities(const EntityFilter& inFilter)
{
	return GetEntities( [&] (const Entity& inEntity) { return inFilter.PassesFilter(inEntity); } );
}
