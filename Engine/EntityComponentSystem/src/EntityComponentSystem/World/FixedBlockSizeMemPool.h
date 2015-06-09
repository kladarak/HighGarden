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

class FixedBlockSizeMemPool
{
private:
	struct ObjectMemoryMover
	{
		void (*mMoveConstructor)(void*, void*);
		void (*mCopyConstructor)(void*, void*);
		void (*mDestructor)(void*);
	};

	template<typename T>
	static inline void sObjectMoveConstructorWrapper(void* inObjectTo, void* inObjectFrom)
	{
		T* objectFrom = static_cast<T*>(inObjectFrom);
		new (inObjectTo) T( std::move( *objectFrom ) );
	}

	template<typename T>
	static inline void sObjectCopyConstructorWrapper(void* inObjectTo, void* inObjectFrom)
	{
		const T* objectFrom = static_cast<const T*>(inObjectFrom);
		new (inObjectTo) T( *objectFrom );
	}

	template<typename T>
	static inline void sObjectDestructorWrapper(void* inObject)
	{
		static_cast<T*>(inObject)->~T();
	}

	template <typename T>
	static ObjectMemoryMover sCreateObjectMover()
	{
		ObjectMemoryMover mover;
		mover.mMoveConstructor	= &sObjectMoveConstructorWrapper<T>;
		mover.mCopyConstructor	= &sObjectCopyConstructorWrapper<T>;
		mover.mDestructor		= &sObjectDestructorWrapper<T>;
		return mover;
	}

public:

	FixedBlockSizeMemPool();
	FixedBlockSizeMemPool(FixedBlockSizeMemPool&& inRHS);
	FixedBlockSizeMemPool(const FixedBlockSizeMemPool& inRHS);
	~FixedBlockSizeMemPool();

	template<typename T>
	void				Init();
	bool				IsInitialised() const						{ return nullptr != mObjectMover.mMoveConstructor; }

	uint32_t			GetCapacity() const							{ return mNumBlocksAllocated; }
	bool				IsAllocated(uint32_t inBlockIndex) const	{ return mUsedBlocks.Get(inBlockIndex); }
	
	template<typename T, typename ...Args>
	T*					Alloc(uint32_t inBlockIndex, Args&&... inArgs);

	void				Dealloc(uint32_t inBlockIndex);
	
	template<typename T>
	T*					Get(uint32_t inBlockIndex) const;

private:
	void				EnsureCapacity(uint32_t inBlockIndex);
	static void*		sGetBlock(void* inMemory, uint32_t inBlockIndex, uint32_t inBlockSize);

	void*				GetBlock(uint32_t inBlockIndex);
	void*				GetBlock(uint32_t inBlockIndex) const;

	ObjectMemoryMover	mObjectMover;
	BitField			mUsedBlocks;

	uint32_t			mNumBlocksAllocated;
	uint32_t			mBlockSize; 
	void*				mMemory;

};

template<typename T>
void FixedBlockSizeMemPool::Init()
{
	mBlockSize		= sizeof(T);
	mObjectMover	= sCreateObjectMover<T>();
}

template<typename T, typename ...Args>
T* FixedBlockSizeMemPool::Alloc(uint32_t inBlockIndex, Args&&... inArgs)
{
	assert(mUsedBlocks.Get(inBlockIndex) == false);	
	assert(mBlockSize == sizeof(T));
	assert(IsInitialised());
	// assert destructor matches what we expect...

	void* block = GetBlock(inBlockIndex);
	new (block) T( std::forward<Args>(inArgs)... );
	mUsedBlocks.Set(inBlockIndex);
	return static_cast<T*>(block);
}

template<typename T>
T* FixedBlockSizeMemPool::Get(uint32_t inBlockIndex) const
{
	assert(mUsedBlocks.Get(inBlockIndex));
	//assert(mBlockSize == sizeof(T));
	// assert destructor matches what we expect...

	void* block = GetBlock(inBlockIndex);
	return static_cast<T*>(block);
}
