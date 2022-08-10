#include "Point.h"

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

	Point magnitude = { 0.0f, 0.0f };
	float module = 0.0f;
};
