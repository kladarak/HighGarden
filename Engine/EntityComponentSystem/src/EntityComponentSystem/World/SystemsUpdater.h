#pragma once

#include <vector>
#include "EntityFilter.h"

class World;
class System;

class SystemsUpdater
{
public:
	void Register(System* inSystem, const EntityFilter& inFilter = EntityFilter());
	void Update(World* inWorld, float inFrameTime);

private:
	struct SystemInfo
	{
		System*			mSystem;
		EntityFilter	mFilter;
	};

	std::vector<SystemInfo> mSystems;
};
