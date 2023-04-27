#include "GasBody.h"

GasBody::GasBody(PhysRect rect, float mass, float pressure, PhysVec dragCoefficient, const float* pixelsToMeters) :
	Body(BodyClass::GAS_BODY, rect, mass, pixelsToMeters)
{
	SetMaterial(Material());

	this->pressure = pressure;
	this->dragCoefficient = dragCoefficient;
}

GasBody::~GasBody()
{
}

void GasBody::SetMaterial(Material material)
{
	Body::SetMaterial(material);

	heatRatio = material.heatRatio;
}

float GasBody::HeatRatio() const
{
	return heatRatio;
}

float GasBody::Pressure() const
{
	return pressure;
}
