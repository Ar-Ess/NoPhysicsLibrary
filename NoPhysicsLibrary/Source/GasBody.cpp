#include "GasBody.h"

GasBody::GasBody(PhysRect rect, float mass, float heatRatio, float pressure, PhysVec dragCoefficient, const float* pixelsToMeters) : Body(BodyClass::GAS_BODY, rect, mass, pixelsToMeters)
{
	this->heatRatio = heatRatio;
	this->pressure = pressure;

	if (dragCoefficient.x < 0) dragCoefficient.x = 0;
	if (dragCoefficient.y < 0) dragCoefficient.y = 0;
	if (dragCoefficient.x > 1.5f) dragCoefficient.x = 1.5f;
	if (dragCoefficient.y > 1.5f) dragCoefficient.y = 1.5f;
	this->dragCoefficient = dragCoefficient;
}

GasBody::~GasBody()
{
}

float GasBody::GetDensity(InUnit unit)
{
	return  mass / GetVolume(unit);
}

float GasBody::GetHeatRatio() const
{
	return heatRatio;
}

float GasBody::GetPressure() const
{
	return pressure;
}

void GasBody::SetDensity(float density, InUnit unit)
{
	if (unit == InUnit::IN_PIXELS) density *= ((1 / *pixelsToMeters) * (1 / *pixelsToMeters));

	this->mass = density * rect.Area();
}

void GasBody::SetHeatRatio(float heatRatio)
{
	this->heatRatio = heatRatio;
}

float GasBody::GetVolume(InUnit unit)
{
	float inPixels = 1;
	if (unit == InUnit::IN_PIXELS) inPixels /= (*pixelsToMeters * *pixelsToMeters);

	return  rect.Area() * inPixels;
}
