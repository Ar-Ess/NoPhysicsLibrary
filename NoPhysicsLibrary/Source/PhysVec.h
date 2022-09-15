#include "Point.h"

struct PhysVec
{
	PhysVec()
	{
		vector = { 0.0f, 0.0f };
		module = 0.0f;
	}

	PhysVec(Point vector)
	{
		this->vector = vector;
		this->module = vector.Module();
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

};

typedef PhysVec Force;
typedef PhysVec Momentum;
