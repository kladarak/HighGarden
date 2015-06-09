#include "EntityComponentSystemTests.h"

#include <Core/Debug/Assert.h>
#include <Core/Debug/Performance.h>

#include <EntityComponentSystem/World/World.h>
#include <EntityComponentSystem/World/EntityBuilder.h>
#include <EntityComponentSystem/World/ComponentsStorage.h>

#include <EntityComponentSystem/Components/PositionComponent.h>

namespace EntityComponentSystemTests
{

class IncNumberComponent
{
public:
	static int sSeed;
	static int sMoveCount;
	static int sCopyCount;

	IncNumberComponent() : mNum(sSeed++) {}
	IncNumberComponent(IncNumberComponent&& inRHS)		: mNum(inRHS.mNum)	{ inRHS.mNum = 0; sMoveCount++; }
	IncNumberComponent(const IncNumberComponent& inRHS) : mNum(inRHS.mNum)	{ sCopyCount++; }

	~IncNumberComponent() { if (mNum != 0) { sSeed--; } }

	void DoubleIt() { mNum *= 2; }

	int mNum;
};

int IncNumberComponent::sSeed = 1;
int IncNumberComponent::sMoveCount = 0;
int IncNumberComponent::sCopyCount = 0;

class VectorComponent
{
public:
	static int sSeed;
	static int sMoveCount;
	static int sCopyCount;

	VectorComponent()								: mNum(sSeed++)											{ mNumbers.resize(20, mNum); }
	VectorComponent(VectorComponent&& inRHS)		: mNum(inRHS.mNum), mNumbers(std::move(inRHS.mNumbers))	{ inRHS.mNum = 0;	sMoveCount++; }
	VectorComponent(const VectorComponent& inRHS)	: mNum(inRHS.mNum), mNumbers(inRHS.mNumbers)			{ sSeed++;			sCopyCount++; }

	~VectorComponent() { if (mNum != 0) { sSeed--; } }

	void DoubleIt() { mNum *= 2; }

	int mNum;
	std::vector<int> mNumbers;
};

int VectorComponent::sSeed = 1;
int VectorComponent::sMoveCount = 0;
int VectorComponent::sCopyCount = 0;


static void CheckEntityComponentComposition();
static void CheckStorageAllocation();

void DoTests()
{
	CheckStorageAllocation();
	CheckEntityComponentComposition();
}

void CheckEntityComponentComposition()
{
	World	world;
	Entity	entity1 = EntityBuilder(world)
						.AddComponent<PositionComponent>(2.0f)
						.AddComponent<VelocityComponent>(-1.0f, 1.0f, 2.0f)
						.Create();

	assert( entity1.HasComponent<PositionComponent>() );
	assert( entity1.HasComponent<VelocityComponent>() );
	assert( entity1.GetComponent<PositionComponent>()->GetPos().x == 2.0f );
	assert( entity1.GetComponent<VelocityComponent>()->GetVelocity().x == -1.0f );

	entity1.RemoveComponent<PositionComponent>();
	assert( !entity1.HasComponent<PositionComponent>() );
	assert( entity1.HasComponent<VelocityComponent>() );

	entity1.Kill();
	assert( !entity1.IsAlive() );
	assert( !entity1.HasComponent<VelocityComponent>() );
}

void CheckStorageAllocation()
{
	time_t t;
	srand((unsigned) time(&t));
	static const int kCount = 10000;

	{
		ScopeTimerUs timer("Alloc/dealloc");

		{
			ComponentsStorage storage;
			for (EntityID i = 0; i < kCount; ++i)
			{
				storage.Alloc<IncNumberComponent>(i);
				storage.Alloc<VectorComponent>(i);
			}
		}
	}

	//printf("Seed = %i, Moves = %i, Copies = %i\n", IncNumberComponent::sSeed, IncNumberComponent::sMoveCount, IncNumberComponent::sCopyCount);
	assert( IncNumberComponent::sSeed == 1 );
	assert( IncNumberComponent::sCopyCount == 0 );
	assert( VectorComponent::sSeed == 1 );
	assert( VectorComponent::sCopyCount == 0 );

	{
		ComponentsStorage storage;
		for (EntityID i = 0; i < kCount; ++i)
		{
			storage.Alloc<IncNumberComponent>(i);
			storage.Alloc<VectorComponent>(i);
		}

		{
			ScopeTimerUs timer("Random access");
			for (int i = 0; i < kCount; ++i)
			{
				EntityID randomID = EntityID(rand()) % kCount;
				storage.Get<IncNumberComponent>(randomID)->DoubleIt();
				storage.Get<VectorComponent>(randomID)->DoubleIt();
			}
		}
	}
}

}
