#include "EntityFilter.h"

#include "World.h"
#include "Entity.h"

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
