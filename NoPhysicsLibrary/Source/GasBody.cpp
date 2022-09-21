#include "GasBody.h"

GasBody::GasBody(Rect rect, float mass, float density, float heatRatio, float pressure) : Body(BodyClass::GAS_BODY, rect, mass)
{
	this->density = density;
	this->heatRatio = heatRatio;
	this->pressure = pressure;
}

GasBody::~GasBody()
{
}
