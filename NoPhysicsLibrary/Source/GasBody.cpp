#include "GasBody.h"

GasBody::GasBody(PhysRect rect, float mass, float heatRatio, float pressure, PhysVec dragCoefficient, const float* pixelsToMeters) : 
	Body(BodyClass::GAS_BODY, rect, mass, pixelsToMeters)
{
	this->heatRatio = heatRatio;
	this->pressure = pressure;
	this->dragCoefficient = dragCoefficient;
}

GasBody::~GasBody()
{
}

float GasBody::Density(InUnit unit) const
{
	return  mass / Volume(unit);
}

float GasBody::HeatRatio() const
{
	return heatRatio;
}

float GasBody::Pressure() const
{
	return pressure;
}

void GasBody::Density(float density, InUnit unit)
{
	if (unit == InUnit::IN_PIXELS) density *= PhysMath::Pow(1 / *pixelsToMeters, 2);

	this->mass = density * rect.Area();
}

void GasBody::HeatRatio(float heatRatio)
{
	this->heatRatio = heatRatio;
}

float GasBody::Volume(InUnit unit) const
{
	float inPixels = 1;
	if (unit == InUnit::IN_PIXELS) inPixels *= PhysMath::Pow(1 / *pixelsToMeters, 2);

	return rect.Area() * inPixels;
}
