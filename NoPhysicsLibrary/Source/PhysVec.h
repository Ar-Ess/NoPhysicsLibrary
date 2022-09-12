#include "Point.h"

//-TODONE: PhysVec base class + empty Force/Momentum sub class as a synonim (buscar manera de fer synonims de classes c++)
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

	//-TODONE: Change name to magnitude to vector
	Point vector = { 0.0f, 0.0f };
	float module = 0.0f;

};

typedef PhysVec Force;
typedef PhysVec Momentum;
