#pragma once

#include "Rect.h"
#include "PhysVec.h"

struct BodyBackup
{
	BodyBackup(Rect rect, Point velocity, Point acceleration, Momentum momentum, Force force)
	{
		this->rect = rect;
		this->velocity = velocity;
		this->acceleration = acceleration;
		this->momentum = momentum;
		this->force = force;
	}

	Rect rect = {};
	Point velocity = { 0, 0 };
	Point acceleration = { 0, 0 };
	Momentum momentum = {};
	Force force = {};
};
