#pragma once

#include "PhysRect.h"

struct BodyBackup
{
	BodyBackup()
	{
		this->rectangle = {0, 0, 0, 0};
		this->velocity = 0;
		this->acceleration = 0;
		this->momentum = 0;
		this->force = 0;
	}

	BodyBackup(PhysRect rectangle, PhysVec velocity, PhysVec acceleration, Momentum momentum, Force force)
	{
		this->rectangle = rectangle;
		this->velocity = velocity;
		this->acceleration = acceleration;
		this->momentum = momentum;
		this->force = force;
	}

	PhysRect rectangle = {};
	PhysVec velocity = { 0.0f, 0.0f };
	PhysVec acceleration = { 0.0f, 0.0f };
	Momentum momentum = {};
	Force force = {};
};
