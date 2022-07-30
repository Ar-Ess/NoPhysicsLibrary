#pragma once

#include "Rect.h"

struct BodyBackUp
{
	BodyBackUp(Rect rect, Point velocity, Point acceleration, Point momentum, Point forces)
	{
		this->rect = rect;
		this->velocity = velocity;
		this->acceleration = acceleration;
		this->momentum = momentum;
		this->forces = forces;
	}

	Rect rect = {};
	Point velocity = { 0, 0 };
	Point acceleration = { 0, 0 };
	Point momentum = { 0, 0 };
	Point forces = { 0, 0 };
};
