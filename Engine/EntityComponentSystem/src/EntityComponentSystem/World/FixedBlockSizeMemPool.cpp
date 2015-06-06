#include "FixedBlockSizeMemPool.h"

#include <cstdlib>
#include <stdlib.h>

#include "Assert.h"

FixedBlockSizeMemPool::FixedBlockSizeMemPool()
	: mBlockSize(0)
	, mNumBlocksAllocated(0)
	, mMemory(nullptr)
{
}

FixedBlockSizeMemPool::FixedBlockSizeMemPool(uint32_t inBlockSize)
	: mBlockSize(inBlockSize)
	, mNumBlocksAllocated(0)
	, mMemory(nullptr)
{
}
	
FixedBlockSizeMemPool::~FixedBlockSizeMemPool()
{
	free(mMemory);
}

void* FixedBlockSizeMemPool::GetBlock(uint32_t inBlockIndex)
{
	assert(mBlockSize > 0);
	EnsureCapacity(inBlockIndex);
	return (void*) ((unsigned char*) mMemory + (inBlockIndex*mBlockSize));
}

void* FixedBlockSizeMemPool::GetBlock(uint32_t inBlockIndex) const
{
	assert(mBlockSize > 0);
	assert(inBlockIndex < mNumBlocksAllocated);
	return (void*) ((unsigned char*) mMemory + (inBlockIndex*mBlockSize));
}

void FixedBlockSizeMemPool::EnsureCapacity(uint32_t inBlockIndex)
{
	if (inBlockIndex >= mNumBlocksAllocated)
	{
		mNumBlocksAllocated *= 2;
		mNumBlocksAllocated += 2; // Handle first alloc by increasing from 0.
		mMemory = realloc(mMemory, mNumBlocksAllocated * mBlockSize);
		assert(mMemory);
	}
}