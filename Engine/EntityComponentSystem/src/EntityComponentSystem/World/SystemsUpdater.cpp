#include "SystemsUpdater.h"

#include "World.h"
#include "Entity.h"
#include "System.h"


void SystemsUpdater::Register(System* inSystem, const EntityFilter& inFilter)
{
	SystemInfo info = { inSystem, inFilter };
	mSystems.push_back(info);
}

void SystemsUpdater::Update(World* inWorld, float inFrameTime)
{
	std::vector<Entity> entities;

	for (auto& systemInfo : mSystems)
	{
		inWorld->ForEachEntity( [&] (EntityID inEntityID)
		{
			Entity entity(inEntityID, inWorld);

			if (systemInfo.mFilter.PassesFilter(entity))
			{
				entities.push_back(entity);
			}
		} );
		
		systemInfo.mSystem->Update( entities, inFrameTime );

		entities.clear();
	}
}
