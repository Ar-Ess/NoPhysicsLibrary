#pragma once

#include "Physics.h"
#include "Audio.h"
#include "StaticBody.h"
#include "DynamicBody.h"
#include "LiquidBody.h"
#include "GasBody.h"

typedef double Float;

struct BodyCreation
{
	BodyCreation(Rect rect, float mass, std::vector<Body*>* bodies, std::vector<unsigned int*>* gasLocation,Physics* physics, const float* pixelsToMeters)
	{
		this->mass = mass;
		this->rect = rect;
		this->bodies = bodies;
		this->gasLocation = gasLocation;
		this->physics = physics;
		this->pixelsToMeters = pixelsToMeters;
	}

	StaticBody* Static()
	{
		bodies->emplace_back(new StaticBody(rect, mass, pixelsToMeters));
		return (StaticBody*)bodies->back();
	}

	DynamicBody* Dynamic(Point gravityOffset = {0.0f, 0.0f }, InUnit unit = InUnit::IN_METERS)
	{
		if (!gravityOffset.IsZero() && unit == InUnit::IN_PIXELS) gravityOffset *= *pixelsToMeters;

		bodies->emplace_back(new DynamicBody(rect, gravityOffset, mass, &physics->globals, pixelsToMeters));
		return (DynamicBody*)bodies->back();
	}

	LiquidBody* Liquid()
	{
		bodies->emplace_back(new LiquidBody(rect, mass, pixelsToMeters));
		return (LiquidBody*)bodies->back();
	}

	GasBody* Gas(float density, float heatRatio, float pressure)
	{
		bodies->emplace_back(new GasBody(rect, mass, density, heatRatio, pressure, pixelsToMeters));
		gasLocation->emplace_back(new unsigned int(bodies->size() - 1));
		return (GasBody*)bodies->back();
	}

private:

	float mass = 1.0f;
	Rect rect = {};
	std::vector<Body*>* bodies = nullptr;
	std::vector<unsigned int*>* gasLocation = nullptr;
	Physics* physics = nullptr;
	const float* pixelsToMeters = nullptr;
};
