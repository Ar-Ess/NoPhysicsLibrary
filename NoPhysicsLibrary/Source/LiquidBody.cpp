#include "LiquidBody.h"

LiquidBody::LiquidBody(Rect rect, float mass, float buoyancy, const float* pixelsToMeters) : Body(BodyClass::LIQUID_BODY, rect, mass, pixelsToMeters)
{
	buoyancy < 0 ? this->buoyancy = 0 : buoyancy > 1 ? this->buoyancy = 1 : this->buoyancy = buoyancy;
}

LiquidBody::~LiquidBody()
{
}

float LiquidBody::GetVolume(InUnit unit)
{
	float inPixels = 1;
	if (unit == InUnit::IN_PIXELS) inPixels /= (*pixelsToMeters * *pixelsToMeters);

	return  rect.GetArea() * inPixels;
}

// kg / m^2
float LiquidBody::GetDensity(InUnit unit)
{
	return  mass / GetVolume(unit);
}

float LiquidBody::GetBuoyancy()
{
	return buoyancy;
}

void LiquidBody::SetDensity(float density, InUnit unit)
{
	if (unit == InUnit::IN_PIXELS) density *= ((1 / *pixelsToMeters) * (1 / *pixelsToMeters));

	this->mass = density * rect.GetArea();
}

void LiquidBody::SetBuoyancy(float buoyancy)
{
	buoyancy < 0 ? this->buoyancy = 0 : buoyancy > 1 ? this->buoyancy = 1 : this->buoyancy = buoyancy;
}
