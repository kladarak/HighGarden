#include "SystemsUpdater.h"

#include "World.h"
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
		inWorld->ForEachEntity( [&] (Entity inEntityID)
		{
			if (systemInfo.mFilter.PassesFilter(inEntityID))
			{
				entities.push_back(inEntityID);
			}
		} );
		
		systemInfo.mSystem->Update( entities, inFrameTime );

		entities.clear();
	}
}
