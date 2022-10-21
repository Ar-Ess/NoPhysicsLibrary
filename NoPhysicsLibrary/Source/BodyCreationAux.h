#pragma once

#include "Physics.h"
#include "Audio.h"
#include "StaticBody.h"
#include "DynamicBody.h"
#include "LiquidBody.h"
#include "GasBody.h"

struct BodyCreation
{
private:

	BodyCreation(Rect rect, float mass, std::vector<Body*>* bodies, std::vector<unsigned int*>* gasLocation, std::vector<unsigned int*>* liquidIndex, Physics* physics, const float* pixelsToMeters, Flag* bodiesConfig) :
		mass(mass),
		rect(rect),
		bodies(bodies),
		gasIndex(gasLocation),
		liquidIndex(liquidIndex),
		physics(physics),
		pixelsToMeters(pixelsToMeters),
		bodiesConfig(bodiesConfig)
	{}

	friend class NPL;

public:

	StaticBody* Static() const
	{
		bodies->emplace_back(new StaticBody(rect, mass, bodiesConfig, pixelsToMeters));
		return (StaticBody*)bodies->back();
	}

	DynamicBody* Dynamic(Point gravityOffset = {0.0f, 0.0f }, InUnit unit = InUnit::IN_METERS) const
	{
		if (!gravityOffset.IsZero() && unit == InUnit::IN_PIXELS) gravityOffset *= *pixelsToMeters;

		bodies->emplace_back(new DynamicBody(rect, gravityOffset, mass, bodiesConfig, &physics->globals, pixelsToMeters));
		return (DynamicBody*)bodies->back();
	}

	LiquidBody* Liquid() const
	{
		bodies->emplace_back(new LiquidBody(rect, mass, bodiesConfig, pixelsToMeters));
		liquidIndex->emplace_back(new unsigned int(bodies->size() - 1));
		return (LiquidBody*)bodies->back();
	}

	GasBody* Gas(float density, float heatRatio, float pressure) const
	{
		bodies->emplace_back(new GasBody(rect, mass, bodiesConfig, density, heatRatio, pressure, pixelsToMeters));
		gasIndex->emplace_back(new unsigned int(bodies->size() - 1));
		return (GasBody*)bodies->back();
	}

private:

	float mass = 1.0f;
	Rect rect = {};
	std::vector<Body*>* bodies = nullptr;
	std::vector<unsigned int*>* gasIndex = nullptr;
	std::vector<unsigned int*>* liquidIndex = nullptr;
	Physics* physics = nullptr;
	const float* pixelsToMeters = nullptr;
	Flag* bodiesConfig = nullptr;
};
