#include "ComponentsStorage.h"

ComponentsStorage::~ComponentsStorage()
{
}

void ComponentsStorage::EnsureCapacity(ComponentTypeID inTypeID)
{
	if (inTypeID >= mMemPools.size())
	{
		mMemPools.resize(inTypeID+1);
	}
}

void ComponentsStorage::RemoveAllComponents(EntityID inID)
{
	for (auto& pool : mMemPools)
	{
		if (pool.IsAllocated(inID))
		{
			pool.Dealloc(inID);
		}
	}
}

bool ComponentsStorage::Has(EntityID inID, ComponentTypeID inTypeID) const
{
	return HasPool(inTypeID) && GetPool(inTypeID).IsAllocated(inID);
}

bool ComponentsStorage::HasPool(ComponentTypeID inTypeID) const
{
	return (mMemPools.size() > inTypeID) 
			&& GetPool(inTypeID).IsInitialised(); // THIS part is surprisingly slow?
}

FixedBlockSizeMemPool& ComponentsStorage::GetPool(ComponentTypeID inTypeID)
{
	auto& pool = static_cast< const ComponentsStorage* >(this)->GetPool(inTypeID);
	return *const_cast< FixedBlockSizeMemPool* >(&pool);
}

const FixedBlockSizeMemPool& ComponentsStorage::GetPool(ComponentTypeID inTypeID) const
{
	assert(inTypeID < mMemPools.size());
	return mMemPools[inTypeID];
}
