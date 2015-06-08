#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "Assert.h"

#include <Core/Containers/BitField.h>

/*
	This is a memory pool for allocating constant size blocks.
	It may perform reallocation underneath, so never hold onto 
	pointers to memory allocated grabbed from this.
*/
template<typename T>
class FixedBlockSizeMemPool
{
public:
	FixedBlockSizeMemPool();
	~FixedBlockSizeMemPool();

	uint32_t	GetCapacity() const							{ return mNumBlocksAllocated; }
	bool		IsAllocated(uint32_t inBlockIndex) const	{ return mUsedBlocks.Get(inBlockIndex); }
	
	template<typename ...Args>
	void		Alloc(uint32_t inBlockIndex, Args&&... inArgs);

	void		Dealloc(uint32_t inBlockIndex);
	
	T*			Get(uint32_t inBlockIndex) const;

private:
	uint32_t	GetBlockSize() const						{ return sizeof(T); }
	T*			GetBlock(uint32_t inBlockIndex);
	T*			GetBlock(uint32_t inBlockIndex) const;
	
	void		EnsureCapacity(uint32_t inBlockIndex);

	BitField	mUsedBlocks;
	uint32_t	mNumBlocksAllocated;
	void*		mMemory;

};

template<typename T>
FixedBlockSizeMemPool<T>::FixedBlockSizeMemPool()
	: mNumBlocksAllocated(0)
	, mMemory(nullptr)
{
}
	
template<typename T>
FixedBlockSizeMemPool<T>::~FixedBlockSizeMemPool()
{
	for (uint32_t i = 0; i < mUsedBlocks.NumBits(); ++i)
	{
		if (mUsedBlocks.Get(i))
		{
			Dealloc(i);
		}
	}

	free(mMemory);
}

template<typename T>
template<typename ...Args>
void FixedBlockSizeMemPool<T>::Alloc(uint32_t inBlockIndex, Args&&... inArgs)
{
	assert(mUsedBlocks.Get(inBlockIndex) == false);	
	new (GetBlock(inBlockIndex)) T( std::forward<Args>(inArgs)... );
	mUsedBlocks.Set(inBlockIndex);
}

template<typename T>
void FixedBlockSizeMemPool<T>::Dealloc(uint32_t inBlockIndex)
{
	Get(inBlockIndex)->~T();
	mUsedBlocks.Clear(inBlockIndex);
}

template<typename T>
T* FixedBlockSizeMemPool<T>::Get(uint32_t inBlockIndex) const
{
	assert(mUsedBlocks.Get(inBlockIndex));
	return GetBlock(inBlockIndex);
}

template<typename T>
inline void* gGetBlock(void* inMemory, uint32_t inBlockIndex, uint32_t inCapacity)
{
	assert(inBlockIndex < inCapacity);
	return (void*) ((unsigned char*) inMemory + (inBlockIndex * sizeof(T)));
}

template<typename T>
T* FixedBlockSizeMemPool<T>::GetBlock(uint32_t inBlockIndex)
{
	EnsureCapacity(inBlockIndex);
	return static_cast<T*>( gGetBlock<T>(mMemory, inBlockIndex, mNumBlocksAllocated) );
}

template<typename T>
T* FixedBlockSizeMemPool<T>::GetBlock(uint32_t inBlockIndex) const
{
	return static_cast<T*>( gGetBlock<T>(mMemory, inBlockIndex, mNumBlocksAllocated) );
}

template<typename T>
void FixedBlockSizeMemPool<T>::EnsureCapacity(uint32_t inBlockIndex)
{
	if (inBlockIndex >= mNumBlocksAllocated)
	{
		// Reserve more space, for at least this block, with optimisation for possible further allocs
		// (optimisation is not measured or based on anything other than a rough guess at what might be reasonable).

		uint32_t	oldAllocCount	= mNumBlocksAllocated;
		void*		oldMemPtr		= mMemory;

		mNumBlocksAllocated = (inBlockIndex + 1) + (mNumBlocksAllocated / 2);
		mMemory				= malloc( mNumBlocksAllocated * GetBlockSize() );
		assert(mMemory);
	
		for (uint32_t i = 0; i < oldAllocCount; ++i)
		{
			if (mUsedBlocks.Get(i))
			{
				T* oldObject = static_cast<T*>( gGetBlock<T>(oldMemPtr, i, oldAllocCount) );
				Alloc(i, std::move(*oldObject));
			}
		}

		free(oldMemPtr);
	}
}
