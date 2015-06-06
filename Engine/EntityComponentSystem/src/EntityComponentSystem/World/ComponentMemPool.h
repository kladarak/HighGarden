#pragma once

#include <Core/Debug/Assert.h>
#include <Core/Containers/BitField.h>

#include "FixedBlockSizeMemPool.h"
#include "EntityTypedef.h"

class ComponentMemPoolBase
{
public:
	virtual					~ComponentMemPoolBase() { }

	virtual void			Dealloc(EntityID inID) = 0;

	bool					IsInitialised() const		{ return mPool.GetBlockSize() > 0; }
	bool					Has(EntityID inID) const	{ return mUsedBlocks.Get(inID); }

protected:
	FixedBlockSizeMemPool	mPool;
	BitField				mUsedBlocks;
};

template<typename T>
class ComponentMemPool : public ComponentMemPoolBase
{
public:
					ComponentMemPool();
	virtual			~ComponentMemPool();

	template<typename ...Args>
	void			Alloc(EntityID inID, Args&&... inArgs);

	virtual void	Dealloc(EntityID inID);

	T*				Get(EntityID inID) const;
};

template<typename T>
ComponentMemPool<T>::ComponentMemPool()
{
	mPool		= FixedBlockSizeMemPool(sizeof(T));
	mUsedBlocks = BitField();
}

template<typename T>
ComponentMemPool<T>::~ComponentMemPool()
{
	for (EntityID i = 0; i < mUsedBlocks.NumBits(); ++i)
	{
		if (mUsedBlocks.Get(i))
		{
			Dealloc(i);
		}
	}

	mPool		= FixedBlockSizeMemPool();
	mUsedBlocks = BitField();
}

template<typename T>
template<typename ...Args>
void ComponentMemPool<T>::Alloc(EntityID inID, Args&&... inArgs)
{
	assert(mUsedBlocks.Get(inID) == false);	
	new (mPool.GetBlock(inID)) T(inArgs...);
	mUsedBlocks.Set(inID);
}

template<typename T>
void ComponentMemPool<T>::Dealloc(EntityID inID)
{
	Get(inID)->~T();
	mUsedBlocks.Clear(inID);
}

template<typename T>
T* ComponentMemPool<T>::Get(EntityID inID) const
{
	assert(mUsedBlocks.Get(inID));
	return static_cast<T*>(mPool.GetBlock(inID));
}
