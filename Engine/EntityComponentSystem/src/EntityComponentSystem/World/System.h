#pragma once

#include <vector>

class Entity;

class System
{
public:
	virtual ~System() { }

	virtual void Update(const std::vector<Entity>& inEntities, float inFrameTime) = 0;
};