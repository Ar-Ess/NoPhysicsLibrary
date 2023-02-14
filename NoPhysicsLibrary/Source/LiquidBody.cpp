#include "LiquidBody.h"
#include "PhysMath.h"

LiquidBody::LiquidBody(PhysRect rect, float mass, float buoyancy, const float* pixelsToMeters) : 
	Body(BodyClass::LIQUID_BODY, rect, mass, pixelsToMeters)
{
	this->buoyancy = buoyancy;
}

LiquidBody::~LiquidBody()
{
}

float LiquidBody::Volume(InUnit unit) const
{
	float inPixels = 1;
	if (unit == InUnit::IN_PIXELS) inPixels *= PhysMath::Pow(1 / *pixelsToMeters, 2);

	return  rect.Area() * inPixels;
}

float LiquidBody::Density(InUnit unit) const
{
	return  mass / Volume(unit);
}

float LiquidBody::Buoyancy() const
{
	return buoyancy;
}

void LiquidBody::Density(float density, InUnit unit)
{
	if (unit == InUnit::IN_PIXELS) density *= PhysMath::Pow( 1 / *pixelsToMeters, 2);

	this->mass = density * rect.Area();
}

void LiquidBody::Buoyancy(float buoyancy)
{
	this->buoyancy = buoyancy;
}
