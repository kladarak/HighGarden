#pragma once

#include "World.h"

class EntityBuilder
{
public:
	EntityBuilder(World& inWorld) : mEntity(inWorld.CreateEntity()) { }

	template<typename T, typename ...Args>
	EntityBuilder& AddComponent(Args&&... inArgs)
	{
		mEntity.AddComponent<T>(std::forward<Args>(inArgs)...);
		return *this;
	}

	Entity Create() { return mEntity; }

private:
	Entity mEntity;
};
