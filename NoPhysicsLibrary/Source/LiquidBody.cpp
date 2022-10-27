#include "LiquidBody.h"

LiquidBody::LiquidBody(Rect rect, float mass, float buoyancy, Flag* bodiesConfig, const float* pixelsToMeters) : Body(BodyClass::LIQUID_BODY, rect, mass, bodiesConfig, pixelsToMeters)
{
	buoyancy < 0 ? this->buoyancy = 0 : buoyancy > 1 ? this->buoyancy = 1 : this->buoyancy = buoyancy;
}

LiquidBody::~LiquidBody()
{
}

float LiquidBody::GetVolume(InUnit unit)
{
	float inPixels = 1;
	if (unit == InUnit::IN_PIXELS) inPixels /= *pixelsToMeters;

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
