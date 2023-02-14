#pragma once

#include "StaticBody.h"
#include "DynamicBody.h"
#include "LiquidBody.h"
#include "GasBody.h"

struct BodyCreation
{
private:

	BodyCreation(PhysArray<Body*>* bodies, PhysArray<unsigned int*>* gasLocation, PhysArray<unsigned int*>* liquidIndex, Flag* globals, const float* pixelsToMeters) :
		bodies(bodies),
		gasIndex(gasLocation),
		liquidIndex(liquidIndex),
		globals(globals),
		pixelsToMeters(pixelsToMeters)
	{}

	const BodyCreation* Access(PhysRect rect)
	{
		this->rect = rect;
		return this;
	}

	friend class NPL;

public:

	StaticBody* Static() const
	{
		StaticBody* b = new StaticBody(rect, 1.0f, pixelsToMeters);
		bodies->Add(b);
		return b;
	}

	// mass is a value that tipically must not be 0 or negative ;)
	DynamicBody* Dynamic(float mass, PhysVec gravityOffset = { 0.0f, 0.0f }, InUnit gravityUnit = InUnit::IN_METERS) const
	{
		// pxl^2/s --> m^2/s
		if (gravityUnit == InUnit::IN_PIXELS) gravityOffset *= PhysMath::Pow(*pixelsToMeters, 2);

		DynamicBody* b = new DynamicBody(rect, gravityOffset, mass, globals, pixelsToMeters);
		bodies->Add(b);
		return b;
	}

	// Buoyancy is a value ranged tipically from 0 to 1 ;)
	LiquidBody* Liquid(float mass, float buoyancy) const
	{
		LiquidBody* b = new LiquidBody(rect, mass, buoyancy, pixelsToMeters);
		bodies->Add(b);
		liquidIndex->Add(new unsigned int(bodies->Size() - 1));
		return b;
	}
	// Buoyancy is a value ranged tipically from 0 to 1 ;)
	LiquidBody* Liquid(float density, float buoyancy, InUnit densityUnit) const
	{
		if (densityUnit == InUnit::IN_PIXELS) density *= PhysMath::Pow((1 / *pixelsToMeters), 2);

		LiquidBody* b = new LiquidBody(rect, density * rect.Area(), buoyancy, pixelsToMeters);
		bodies->Add(b);
		liquidIndex->Add(new unsigned int(bodies->Size() - 1));
		return b;
	}

	// Aerodrag Coefficient is a value tipically ranged from 0 to 1.5 ;)
	GasBody* Gas(float density, float heatRatio, float pressure, PhysVec aerodragCoefficient, InUnit densityUnit, InUnit pressureUnit) const
	{
		float pixToMetSquared = PhysMath::Pow((1 / *pixelsToMeters), 2);
		if (densityUnit == InUnit::IN_PIXELS) density *= pixToMetSquared;
		if (pressureUnit == InUnit::IN_PIXELS) pressure *= pixToMetSquared;

		GasBody* b = new GasBody(rect, density * rect.Area(), heatRatio, pressure, aerodragCoefficient, pixelsToMeters);
		bodies->Add(b);
		gasIndex->Add(new unsigned int(bodies->Size() - 1));
		return b;
	}

private:

	PhysRect rect = {};
	PhysArray<Body*>* bodies = nullptr;
	PhysArray<unsigned int*>* gasIndex = nullptr;
	PhysArray<unsigned int*>* liquidIndex = nullptr;
	const float* pixelsToMeters = nullptr;
	Flag* globals = nullptr;
	Flag* bodiesConfig = nullptr;
};
