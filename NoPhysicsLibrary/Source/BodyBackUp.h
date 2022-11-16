#pragma once

#include "PhysVec.h"

struct BodyBackup
{
	BodyBackup()
	{
		this->rectangle = {};
		this->velocity = {};
		this->acceleration = {};
		this->momentum = {};
		this->force = {};
	}

	BodyBackup(Rect rectangle, Point velocity, Point acceleration, Momentum momentum, Force force)
	{
		this->rectangle = rectangle;
		this->velocity = velocity;
		this->acceleration = acceleration;
		this->momentum = momentum;
		this->force = force;
	}

	Rect rectangle = {};
	Point velocity = { 0.0f, 0.0f };
	Point acceleration = { 0.0f, 0.0f };
	Momentum momentum = {};
	Force force = {};
};
