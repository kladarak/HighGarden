#pragma once

#include <vector>

#include "ComponentTypeID.h"

class World;
class Entity;

class EntityFilter
{
public:
	EntityFilter();

	template<typename T>
	EntityFilter&		MustHave() { mComponentTypes.push_back( T::sGetTypeID() ); return *this; }

	bool				PassesFilter(const Entity& inEntity) const;

private:
	std::vector<ComponentTypeID> mMustHaveComponents;

};
