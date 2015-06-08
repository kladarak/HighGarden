#include "EntityFilter.h"

#include "World.h"

bool EntityFilter::PassesFilter(const Entity& inEntity) const
{
	for (ComponentTypeID typeID : mMustHaveComponents)
	{
		if ( !inEntity.HasComponent(typeID) )
		{
			return false;
		}
	}

	return true;
}
