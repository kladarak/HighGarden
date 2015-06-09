#include "FixedBlockSizeMemPool.h"


FixedBlockSizeMemPool::FixedBlockSizeMemPool()
	: mNumBlocksAllocated(0)
	, mBlockSize(0)
	, mMemory(nullptr)
{
	mObjectMover.mMoveConstructor	= nullptr;
	mObjectMover.mCopyConstructor	= nullptr;
	mObjectMover.mDestructor		= nullptr;
}

FixedBlockSizeMemPool::FixedBlockSizeMemPool(FixedBlockSizeMemPool&& inRHS)
	: mObjectMover			( inRHS.mObjectMover			) 
	, mUsedBlocks			( std::move(inRHS.mUsedBlocks)	)
	, mNumBlocksAllocated	( inRHS.mNumBlocksAllocated		)
	, mBlockSize			( inRHS.mBlockSize				)
	, mMemory				( inRHS.mMemory					)
{
	inRHS.mObjectMover.mMoveConstructor	= nullptr;
	inRHS.mObjectMover.mCopyConstructor	= nullptr;
	inRHS.mObjectMover.mDestructor		= nullptr;
	inRHS.mNumBlocksAllocated			= 0;
	inRHS.mBlockSize					= 0;
	inRHS.mMemory						= nullptr;
}

FixedBlockSizeMemPool::FixedBlockSizeMemPool(const FixedBlockSizeMemPool& inRHS)
	: mObjectMover			( inRHS.mObjectMover			)
	, mUsedBlocks			( inRHS.mUsedBlocks				)
	, mNumBlocksAllocated	( inRHS.mNumBlocksAllocated		)
	, mBlockSize			( inRHS.mBlockSize				)
	, mMemory				( nullptr						)
{
	mMemory	= malloc( mNumBlocksAllocated * mBlockSize );
	assert(mMemory);
	
	for (uint32_t i = 0; i < mUsedBlocks.NumBits(); ++i)
	{
		if (mUsedBlocks.Get(i))
		{
			mObjectMover.mCopyConstructor( GetBlock(i), inRHS.GetBlock(i) );
		}
	}
}

FixedBlockSizeMemPool::~FixedBlockSizeMemPool()
{
	for (uint32_t i = 0; i < mUsedBlocks.NumBits(); ++i)
	{
		if (mUsedBlocks.Get(i))
		{
			mObjectMover.mDestructor( GetBlock(i) );
		}
	}

	free(mMemory);
}

void* FixedBlockSizeMemPool::sGetBlock(void* inMemory, uint32_t inBlockIndex, uint32_t inBlockSize)
{
	return (void*) ((unsigned char*) inMemory + (inBlockIndex * inBlockSize));
}

void* FixedBlockSizeMemPool::GetBlock(uint32_t inBlockIndex)
{
	EnsureCapacity(inBlockIndex);
	assert(inBlockIndex < mNumBlocksAllocated);
	return sGetBlock(mMemory, inBlockIndex, mBlockSize);
}

void* FixedBlockSizeMemPool::GetBlock(uint32_t inBlockIndex) const
{
	assert(inBlockIndex < mNumBlocksAllocated);
	return sGetBlock(mMemory, inBlockIndex, mBlockSize);
}

void FixedBlockSizeMemPool::EnsureCapacity(uint32_t inBlockIndex)
{
	if (inBlockIndex >= mNumBlocksAllocated)
	{
		// Reserve more space, for at least this block, with optimisation for possible further allocs
		// (optimisation is not measured or based on anything other than a rough guess at what might be reasonable).

		uint32_t	oldAllocCount	= mNumBlocksAllocated;
		void*		oldMemPtr		= mMemory;

		mNumBlocksAllocated = (inBlockIndex + 1) + (mNumBlocksAllocated / 2);
		mMemory				= malloc( mNumBlocksAllocated * mBlockSize );
		assert(mMemory);
	
		for (uint32_t i = 0; i < oldAllocCount; ++i)
		{
			if (mUsedBlocks.Get(i))
			{
				void* oldObject = sGetBlock(oldMemPtr, i, mBlockSize);
				void* newObject = GetBlock(i);
				mObjectMover.mMoveConstructor(newObject, oldObject);
				mObjectMover.mDestructor(oldObject);
			}
		}

		free(oldMemPtr);
	}
}

void FixedBlockSizeMemPool::Dealloc(uint32_t inBlockIndex)
{
	assert(mUsedBlocks.Get(inBlockIndex));
	void* block = GetBlock(inBlockIndex);
	mObjectMover.mDestructor(block);
	mUsedBlocks.Clear(inBlockIndex);
}
