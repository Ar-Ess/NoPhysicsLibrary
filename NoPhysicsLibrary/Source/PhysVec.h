#include "Point.h"
#include "InUnitsEnum.h"

struct PhysVec
{
	PhysVec()
	{
		vector = { 0.0f, 0.0f };
		module = 0.0f;
	}

	PhysVec(Point vector, InUnit unit)
	{
		this->vector = vector;
		this->module = vector.Module();
		this->unit = unit;
	}

	PhysVec(float x, float y, InUnit unit)
	{
		this->vector = Point{x, y};
		this->module = vector.Module();
		this->unit = unit;
	}

	~PhysVec()
	{

	}

	void Clear()
	{
		vector.Zero();
		module = 0.0f;
	}

	Point vector = { 0.0f, 0.0f };
	float module = 0.0f;
	InUnit unit = InUnit::IN_METERS;

};

typedef PhysVec Force;
typedef PhysVec Momentum;
