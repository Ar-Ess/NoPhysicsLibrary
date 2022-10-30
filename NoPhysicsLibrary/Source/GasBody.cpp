#include "GasBody.h"

GasBody::GasBody(Rect rect, float mass, float heatRatio, float pressure, const float* pixelsToMeters) : Body(BodyClass::GAS_BODY, rect, mass, pixelsToMeters)
{
	this->heatRatio = heatRatio;
	this->pressure = pressure;
}

GasBody::~GasBody()
{
}
