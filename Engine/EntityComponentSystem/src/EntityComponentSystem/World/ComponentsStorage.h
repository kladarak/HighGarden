#pragma once

#include "ComponentTypeID.h"
#include "EntityTypedef.h"
#include "FixedBlockSizeMemPool.h"

class ComponentsStorage
{
public:
	~ComponentsStorage();
	
	template<typename T, typename ...Args>
	void	Alloc(EntityID inID, Args&&... inArgs);

	template<typename T>
	void	Dealloc(EntityID inID);

	template<typename T>
	bool	Has(EntityID inID) const;

	bool	Has(EntityID inID, ComponentTypeID inTypeID) const;

	template<typename T>
	T*		Get(EntityID inID) const;
	
	void	RemoveAllComponents(EntityID inID);

private:
	void	EnsureCapacity(ComponentTypeID inTypeID);

	template<typename T>
	void	EnsureMemPoolIsInitialised();

	template<typename T>
	bool								HasPool() const;
	bool								HasPool(ComponentTypeID inTypeID) const;

	template<typename T>
	FixedBlockSizeMemPool&				GetPool();
	FixedBlockSizeMemPool&				GetPool(ComponentTypeID inTypeID);

	template<typename T>
	const FixedBlockSizeMemPool&		GetPool() const;
	const FixedBlockSizeMemPool&		GetPool(ComponentTypeID inTypeID) const;

	std::vector<FixedBlockSizeMemPool>	mMemPools;
};
	
template<typename T, typename ...Args>
void ComponentsStorage::Alloc(EntityID inID, Args&&... inArgs)
{
	assert( !Has<T>(inID) );

	EnsureCapacity( gGetComponentTypeID<T>() );
	EnsureMemPoolIsInitialised<T>();

	GetPool<T>().Alloc<T, Args...>(inID, std::forward<Args>(inArgs)...);
}

template<typename T>
void ComponentsStorage::Dealloc(EntityID inID)
{
	assert( Has<T>(inID) );
	GetPool<T>().Dealloc(inID);
}

template<typename T>
bool ComponentsStorage::Has(EntityID inID) const
{
	return Has( inID, gGetComponentTypeID<T>() );
}

template<typename T>
T* ComponentsStorage::Get(EntityID inID) const
{
	return Has<T>(inID) ? GetPool<T>().Get<T>(inID) : nullptr;
}
	
template<typename T>
void ComponentsStorage::EnsureMemPoolIsInitialised()
{
	auto typeID = gGetComponentTypeID<T>();
	assert(mMemPools.size() > typeID);

	if (!mMemPools[typeID].IsInitialised())
	{
		mMemPools[typeID].Init<T>();
	}
}

template<typename T>
FixedBlockSizeMemPool& ComponentsStorage::GetPool()
{
	return GetPool( gGetComponentTypeID<T>() );
}

template<typename T>
const FixedBlockSizeMemPool& ComponentsStorage::GetPool() const
{
	return GetPool( gGetComponentTypeID<T>() );
}

template<typename T>
bool ComponentsStorage::HasPool() const
{
	return HasPool( gGetComponentTypeID<T>() );
}
