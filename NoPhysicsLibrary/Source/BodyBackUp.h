#pragma once

#include "PhysVec.h"

struct BodyBackup
{
	BodyBackup()
	{
		this->position = {};
		this->velocity = {};
		this->acceleration = {};
		this->momentum = {};
		this->force = {};
	}

	BodyBackup(Point position, Point velocity, Point acceleration, Momentum momentum, Force force)
	{
		this->position = position;
		this->velocity = velocity;
		this->acceleration = acceleration;
		this->momentum = momentum;
		this->force = force;
	}

	Point position = {};
	Point velocity = { 0, 0 };
	Point acceleration = { 0, 0 };
	Momentum momentum = {};
	Force force = {};
};
