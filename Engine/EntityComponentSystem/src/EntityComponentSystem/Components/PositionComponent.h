#pragma once

#include <Core/Containers/String.h>

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3() { }
	Vector3(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ) { }
	Vector3(float inValue) : x(inValue), y(inValue), z(inValue) { }
};

class PositionComponent
{
public:
	PositionComponent() { }
	~PositionComponent() { }
	PositionComponent(float inX, float inY, float inZ) : mVector(inX, inY, inZ) { }
	PositionComponent(const Vector3& inVector) : mVector(inVector) { }
	PositionComponent(Vector3&& inVector) : mVector(inVector) { }
	
	const Vector3& GetPos() const { return mVector; }
	
	std::string ToString() const { return gCreateStringF<128>("Position: %f, %f, %f", mVector.x, mVector.y, mVector.z); }
private:
	Vector3 mVector;
};

class VelocityComponent
{
public:
	VelocityComponent() { }
	~VelocityComponent() { }
	VelocityComponent(float inX, float inY, float inZ) : mVector(inX, inY, inZ) { }
	VelocityComponent(const Vector3& inVector) : mVector(inVector) { }
	VelocityComponent(Vector3&& inVector) : mVector(inVector) { }
	
	const Vector3& GetVelocity() const { return mVector; }
	
	std::string ToString() const { return gCreateStringF<128>("Velocity: %f, %f, %f", mVector.x, mVector.y, mVector.z); }

private:
	Vector3 mVector;
};
