#pragma once

#include "Physics.h"
#include "Audio.h"
#include "StaticBody.h"
#include "DynamicBody.h"
#include "LiquidBody.h"
#include "GasBody.h"

struct BodyCreation
{
	BodyCreation(Rect rect, float mass, std::vector<Body*>* bodies, Physics* physics)
	{
		this->mass = mass;
		this->rect = rect;
		this->bodies = bodies;
		this->physics = physics;
	}

	StaticBody* Static()
	{
		StaticBody* body = new StaticBody(rect, mass);
		bodies->emplace_back(body);
		return (StaticBody*)bodies->back();
	}

	DynamicBody* Dynamic(Point velocity = { 0, 0 }, Point gravityOffset = {0, 0})
	{
		DynamicBody* body = new DynamicBody(rect, velocity, gravityOffset, mass, &physics->globals);
		bodies->emplace_back(body);
		return (DynamicBody*)bodies->back();
	}

	LiquidBody* Liquid()
	{
		LiquidBody* body = new LiquidBody(rect, mass);
		bodies->emplace_back(body);
		return (LiquidBody*)bodies->back();
	}

	GasBody* Gas(float density, float heatRatio, float pressure)
	{
		GasBody* body = new GasBody(rect, mass, density, heatRatio, pressure);
		bodies->emplace_back(body);
		return (GasBody*)bodies->back();
	}

private:

	float mass = 1.0f;
	Rect rect = {};
	std::vector<Body*>* bodies = nullptr;
	Physics* physics = nullptr;
};
