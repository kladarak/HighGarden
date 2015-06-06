#include "ComponentsStorage.h"

ComponentsStorage::~ComponentsStorage()
{
	for (auto pool : mMemPools)
	{
		delete pool;
	}
}

void ComponentsStorage::EnsureCapacity(ComponentTypeID inTypeID)
{
	if (inTypeID >= mMemPools.size())
	{
		mMemPools.resize(inTypeID+1, nullptr);
	}
}

void ComponentsStorage::RemoveAllComponents(EntityID inID)
{
	for (auto pool : mMemPools)
	{
		if (nullptr != pool && pool->Has(inID))
		{
			pool->Dealloc(inID);
		}
	}
}

bool ComponentsStorage::Has(EntityID inID, ComponentTypeID inTypeID) const
{
	return HasPool(inTypeID) && GetPool(inTypeID).Has(inID);
}

bool ComponentsStorage::HasPool(ComponentTypeID inTypeID) const
{
	return (mMemPools.size() > inTypeID) && (nullptr != mMemPools[inTypeID]);
}

ComponentMemPoolBase& ComponentsStorage::GetPool(ComponentTypeID inTypeID)
{
	auto& pool = static_cast< const ComponentsStorage* >(this)->GetPool(inTypeID);
	return *const_cast< ComponentMemPoolBase* >(&pool);
}

const ComponentMemPoolBase&	ComponentsStorage::GetPool(ComponentTypeID inTypeID) const
{
	assert(inTypeID);
	return *mMemPools[inTypeID];
}
