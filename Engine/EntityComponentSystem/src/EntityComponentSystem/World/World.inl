

template<typename T, typename ...Args>
T* Entity::AddComponent(Args&&... inArgs)
{
	mWorld->AddComponent<T>(mID, std::forward<Args>(inArgs)...);
	return GetComponent<T>();
}

template<typename T>
void Entity::RemoveComponent()
{
	mWorld->RemoveComponent<T>(mID);
}
	
template<typename T>
bool Entity::HasComponent() const
{
	return mWorld->HasComponent<T>(mID);
}

template<typename T>
T* Entity::GetComponent() const
{
	return mWorld->GetComponent<T>(mID);
}

//----------------------------------------------------------------------

template<typename T, typename ...Args>
T* World::AddComponent(EntityID inID, Args&&... inArgs)
{
	mComponentStorage.Alloc<T>(inID, std::forward<Args>(inArgs)...);
	return GetComponent<T>(inID);
}

template<typename T>
void World::RemoveComponent(EntityID inID)
{
	mComponentStorage.Dealloc<T>(inID);
}

template<typename T>
bool World::HasComponent(EntityID inID) const
{
	return HasComponent(inID, gGetComponentTypeID<T>());
}

template<typename T>
T* World::GetComponent(EntityID inID) const
{
	return mComponentStorage.Get<T>(inID);
}

template<typename TFunctor>
void World::ForEachEntity(const TFunctor& inFunctor)
{
	for (uint32_t i = 0; i < mUsedIDs.NumBits(); ++i)
	{
		if (mUsedIDs.Get(i))
		{
			inFunctor( Entity(EntityID(i), this) );
		}
	}
}
