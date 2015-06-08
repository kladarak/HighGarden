#pragma once

#include <Core/Debug/Assert.h>

#include "FixedBlockSizeMemPool.h"
#include "EntityTypedef.h"

class ComponentMemPoolBase
{
public:
	virtual						~ComponentMemPoolBase() { }
	virtual void				Dealloc(EntityID inID)		= 0;
	virtual bool				Has(EntityID inID) const	= 0;
};

template<typename T>
class ComponentMemPool : public ComponentMemPoolBase
{
public:
	virtual						~ComponentMemPool() { }

	template<typename ...Args>
	void						Alloc(EntityID inID, Args&&... inArgs)	{ return mPool.Alloc(inID, std::forward<Args>(inArgs)...); }
	virtual void				Dealloc(EntityID inID)					{ return mPool.Dealloc(inID); }
	T*							Get(EntityID inID) const				{ return mPool.Get(inID); }
	virtual bool				Has(EntityID inID) const				{ return mPool.IsAllocated(inID); }
	
private:
	FixedBlockSizeMemPool<T>	mPool;
};
