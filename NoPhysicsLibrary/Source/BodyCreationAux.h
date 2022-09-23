#pragma once

#include "Physics.h"
#include "Audio.h"
#include "StaticBody.h"
#include "DynamicBody.h"
#include "LiquidBody.h"
#include "GasBody.h"

struct BodyCreation
{
	BodyCreation(Rect rect, float mass, std::vector<Body*>* bodies, std::vector<unsigned int*>* gasLocation,Physics* physics)
	{
		this->mass = mass;
		this->rect = rect;
		this->bodies = bodies;
		this->gasLocation = gasLocation;
		this->physics = physics;
	}

	StaticBody* Static()
	{
		bodies->emplace_back(new StaticBody(rect, mass));
		return (StaticBody*)bodies->back();
	}

	DynamicBody* Dynamic(Point velocity = { 0, 0 }, Point gravityOffset = {0, 0})
	{
		bodies->emplace_back(new DynamicBody(rect, velocity, gravityOffset, mass, &physics->globals));
		return (DynamicBody*)bodies->back();
	}

	LiquidBody* Liquid()
	{
		bodies->emplace_back(new LiquidBody(rect, mass));
		return (LiquidBody*)bodies->back();
	}

	GasBody* Gas(float density, float heatRatio, float pressure)
	{
		bodies->emplace_back(new GasBody(rect, mass, density, heatRatio, pressure));
		gasLocation->emplace_back(new unsigned int(bodies->size() - 1));
		return (GasBody*)bodies->back();
	}

private:

	float mass = 1.0f;
	Rect rect = {};
	std::vector<Body*>* bodies = nullptr;
	std::vector<unsigned int*>* gasLocation = nullptr;
	Physics* physics = nullptr;
};
