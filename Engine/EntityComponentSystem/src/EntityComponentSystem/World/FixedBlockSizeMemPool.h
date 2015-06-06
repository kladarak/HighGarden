#pragma once

#include <stdint.h>

/*
	This is a memory pool for allocating constant size blocks.
	It may perform reallocation underneath, so never hold onto 
	pointers to memory allocated grabbed from this.
*/
class FixedBlockSizeMemPool
{
public:
	FixedBlockSizeMemPool();
	FixedBlockSizeMemPool(uint32_t inBlockSize);
	~FixedBlockSizeMemPool();

	uint32_t	GetBlockSize() const { return mBlockSize; }

	void*		GetBlock(uint32_t inBlockIndex);
	void*		GetBlock(uint32_t inBlockIndex) const;

private:
	void		EnsureCapacity(uint32_t inBlockIndex);

	uint32_t	mBlockSize;
	uint32_t	mNumBlocksAllocated;
	void*		mMemory;

};
