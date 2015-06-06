#pragma once

#include <stdint.h>

typedef uint32_t ComponentTypeID;

struct ComponentTypeIDHelpers
{
public:
	template<typename T>
	static ComponentTypeID GetID() 
	{ 
		static const ComponentTypeID kID = GetNextID();
		return kID;
	}
	
private:
	static inline ComponentTypeID GetNextID() { static ComponentTypeID sNextID = 0; return sNextID++; }
};

template<typename T>
inline ComponentTypeID gGetComponentTypeID()
{
	return ComponentTypeIDHelpers::GetID<T>();
}
