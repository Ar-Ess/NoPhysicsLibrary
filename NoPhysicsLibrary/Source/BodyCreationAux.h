#pragma once

#include "Physics.h"
#include "Audio.h"
#include "StaticBody.h"
#include "DynamicBody.h"
#include "LiquidBody.h"
#include "GasBody.h"
#include "MathUtils.h"

struct BodyCreation
{
private:

	BodyCreation(std::vector<Body*>* bodies, std::vector<unsigned int*>* gasLocation, std::vector<unsigned int*>* liquidIndex, Physics* physics, const float* pixelsToMeters) :
		bodies(bodies),
		gasIndex(gasLocation),
		liquidIndex(liquidIndex),
		physics(physics),
		pixelsToMeters(pixelsToMeters)
	{}

	const BodyCreation* Access(Rect rect)
	{
		this->rect = rect;
		return this;
	}

	friend class NPL;

public:

	StaticBody* Static() const
	{
		bodies->emplace_back(new StaticBody(rect, 1.0f, pixelsToMeters));
		return (StaticBody*)bodies->back();
	}

	DynamicBody* Dynamic(float mass, Point gravityOffset = { 0.0f, 0.0f }, InUnit unit = InUnit::IN_METERS) const
	{
		if (!gravityOffset.IsZero() && unit == InUnit::IN_PIXELS) gravityOffset *= *pixelsToMeters;

		bodies->emplace_back(new DynamicBody(rect, gravityOffset, mass, &physics->globals, pixelsToMeters));
		return (DynamicBody*)bodies->back();
	}

	// Buoyancy is a coefficient, goes in between 0 and 1
	LiquidBody* Liquid(float mass, float buoyancy) const
	{
		bodies->emplace_back(new LiquidBody(rect, mass, buoyancy, pixelsToMeters));
		liquidIndex->emplace_back(new unsigned int(bodies->size() - 1));
		return (LiquidBody*)bodies->back();
	}

	LiquidBody* Liquid(float density, float buoyancy, InUnit unit) const
	{
		if (unit == InUnit::IN_PIXELS) density *= MathUtils::Pow((1 / *pixelsToMeters),2);

		bodies->emplace_back(new LiquidBody(rect, density * rect.GetArea(), buoyancy, pixelsToMeters));
		liquidIndex->emplace_back(new unsigned int(bodies->size() - 1));
		return (LiquidBody*)bodies->back();
	}

	// Aerodrag Coefficient is a value from 0 to 1. Out of bounds values will be clamped.
	GasBody* Gas(float density, float heatRatio, float pressure, Point aerodragCoefficient, InUnit unit) const
	{
		if (unit == InUnit::IN_PIXELS)
		{
			float pixToMetSquared = MathUtils::Pow((1 / *pixelsToMeters), 2);
			density *= pixToMetSquared;
			pressure *= pixToMetSquared;
		}
		bodies->emplace_back(new GasBody(rect, density * rect.GetArea(), heatRatio, pressure, aerodragCoefficient, pixelsToMeters));
		gasIndex->emplace_back(new unsigned int(bodies->size() - 1));
		return (GasBody*)bodies->back();
	}

private:

	Rect rect = {};
	std::vector<Body*>* bodies = nullptr;
	std::vector<unsigned int*>* gasIndex = nullptr;
	std::vector<unsigned int*>* liquidIndex = nullptr;
	Physics* physics = nullptr;
	const float* pixelsToMeters = nullptr;
	Flag* bodiesConfig = nullptr;
};
