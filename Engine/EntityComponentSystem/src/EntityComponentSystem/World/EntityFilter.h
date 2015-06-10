#pragma once

#include <vector>

#include "ComponentTypeID.h"

class World;
class Entity;

// TODO: Turn this into basically a tuple, using a variadic template to list required components.
class EntityFilter
{
public:
	template<typename T>
	EntityFilter&		MustHave() { mMustHaveComponents.push_back( gGetComponentTypeID<T>() ); return *this; }

	bool				PassesFilter(const Entity& inEntity) const;

private:
	std::vector<ComponentTypeID> mMustHaveComponents;

};
