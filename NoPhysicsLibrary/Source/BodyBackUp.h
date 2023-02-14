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
