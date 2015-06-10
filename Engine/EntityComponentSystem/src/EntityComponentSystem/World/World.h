#pragma once

#include <vector>

#include <Core/Containers/BitField.h>

#include "EntityTypedef.h"
#include "ComponentTypeID.h"
#include "ComponentsStorage.h"

class World;
class EntityFilter;

class Entity
{
public:
	Entity(EntityID inID, World* inWorld) : mID(inID), mWorld(inWorld) { }

	EntityID	GetID() const		{ return mID; }
	World*		GetWorld() const	{ return mWorld; }
	bool		IsAlive() const;
	void		Kill();

	template<typename T, typename ...Args>
	T*			AddComponent(Args&&... inArgs);

	template<typename T>
	void		RemoveComponent();
	
	template<typename T>
	bool		HasComponent() const;
	bool		HasComponent(ComponentTypeID inTypeID) const;

	template<typename T>
	T*			GetComponent() const;

	bool		operator==(const Entity& inRHS) const { return mID == inRHS.mID && mWorld == inRHS.mWorld; }
	bool		operator!=(const Entity& inRHS) const { return !(*this == inRHS); }

private:
	EntityID	mID;
	World*		mWorld;
};


class World
{
public:
	World();
	~World();

	Entity					CreateEntity();
	bool					IsEntityAlive(EntityID inID) const;
	void					DestroyEntity(EntityID inID);

	template<typename T, typename ...Args>
	T*						AddComponent(EntityID inID, Args&&... inArgs);

	template<typename T>
	void					RemoveComponent(EntityID inID);

	template<typename T>
	bool					HasComponent(EntityID inID) const;
	bool					HasComponent(EntityID inID, ComponentTypeID inTypeID) const;

	template<typename T>
	T*						GetComponent(EntityID inID) const;

	template<typename TFunctor>
	void					ForEachEntity(const TFunctor& inFunctor);

	template<typename TFilterFunctor>
	std::vector<Entity>		GetEntities(const TFilterFunctor& inFilter); 

	std::vector<Entity>		GetEntities(const EntityFilter& inFilter);

private:
	EntityID				mNextID;
	std::vector<EntityID>	mUnusedIDs;
	BitField				mUsedIDs;

	ComponentsStorage		mComponentStorage;
};

#include "World.inl"
