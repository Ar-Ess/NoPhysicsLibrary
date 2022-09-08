#pragma once

struct Momentum
{
	Momentum()
	{
		magnitude = { 0.0f, 0.0f };
		module = 0.0f;
	}

	Momentum(Point magnitude)
	{
		this->magnitude = magnitude;
		this->module = magnitude.Module();
	}

	~Momentum()
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
