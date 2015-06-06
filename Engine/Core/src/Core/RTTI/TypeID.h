#pragma once

typedef const void* TypeID;

template<typename T>
struct TypeIDFactory
{
	static TypeID GetID() { static const TypeIDFactory<T> kID; return &kID; }
};

template<typename T>
inline TypeID gGetTypeID()
{
	return TypeIDFactory<T>::GetID();
}