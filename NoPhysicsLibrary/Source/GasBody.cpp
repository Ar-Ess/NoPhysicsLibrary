#include "GasBody.h"

GasBody::GasBody(Rect rect, float mass, Flag* bodiesConfig, float density, float heatRatio, float pressure, const float* pixelsToMeters) : Body(BodyClass::GAS_BODY, rect, mass, bodiesConfig, pixelsToMeters)
{
	this->density = density;
	this->heatRatio = heatRatio;
	this->pressure = pressure;
}

GasBody::~GasBody()
{
}
