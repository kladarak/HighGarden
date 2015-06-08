#pragma once

#include <stdint.h>
#include "World.h"

class Entity
{
public:
	Entity(EntityID inID, World* inWorld) : mID(inID), mWorld(inWorld) { }

	EntityID	GetID() const		{ return mID; }
	bool		IsAlive() const		{ return mWorld->IsEntityAlive(mID); }
	void		Kill()				{ mWorld->DestroyEntity(mID); }
	World*		GetWorld() const	{ return mWorld; }

	template<typename T, typename ...Args>
	T*			AddComponent(Args&&... inArgs);

	template<typename T>
	void		RemoveComponent();
	
	template<typename T>
	bool		HasComponent() const;
	bool		HasComponent(ComponentTypeID inTypeID) const { return mWorld->HasComponent(mID, inTypeID); }

	template<typename T>
	T*			GetComponent() const;

	bool		operator==(const Entity& inRHS) const { return mID == inRHS.mID && mWorld == inRHS.mWorld; }
	bool		operator!=(const Entity& inRHS) const { return !(*this == inRHS); }

private:
	EntityID	mID;
	World*		mWorld;
};


template<typename T, typename ...Args>
T* Entity::AddComponent(Args&&... inArgs)
{
	mWorld->AddComponent<T>(mID, std::forward<Args>(inArgs)...);
	return GetComponent<T>();
}

template<typename T>
void Entity::RemoveComponent()
{
	mWorld->RemoveComponent<T>(mID);
}
	
template<typename T>
bool Entity::HasComponent() const
{
	return mWorld->HasComponent<T>(mID);
}

template<typename T>
T* Entity::GetComponent() const
{
	return mWorld->GetComponent<T>(mID);
}