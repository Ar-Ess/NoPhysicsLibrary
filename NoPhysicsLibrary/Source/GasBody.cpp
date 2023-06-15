#include "GasBody.h"

GasBody::GasBody(PhysRect rect, float mass, PhysVec dragCoefficient, const float* pixelsToMeters) :
	Body(BodyClass::GAS_BODY, rect, mass, pixelsToMeters)
{
	SetMaterial(Material());

	this->dragCoefficient = dragCoefficient;
}

GasBody::~GasBody()
{
}

void GasBody::SetMaterial(Material material)
{
	Body::SetMaterial(material);

	heatRatio = material.heatRatio;
	pressure = material.pressure;
}

float GasBody::HeatRatio() const
{
	return heatRatio;
}

float GasBody::Pressure() const
{
	return pressure;
}
