#include "Point.h"

//-TODO: PhysVec base class + empty Force/Momentum sub class as a synonim (buscar manera de fer synonims de classes c++)
struct Force
{
	Force()
	{
		magnitude = { 0.0f, 0.0f };
		module = 0.0f;
	}

	Force(Point magnitude)
	{
		this->magnitude = magnitude;
		this->module = magnitude.Module();
	}

	~Force()
	{

	}

	void Clear()
	{
		magnitude.Zero();
		module = 0.0f;
	}

	//-TODO: Change name to magnitude to vector
	Point magnitude = { 0.0f, 0.0f };
	float module = 0.0f;

};
