#include "GasBody.h"

GasBody::GasBody(Rect rect, float mass, Flag* bodiesConfig, float heatRatio, float pressure, const float* pixelsToMeters) : Body(BodyClass::GAS_BODY, rect, mass, bodiesConfig, pixelsToMeters)
{
	this->heatRatio = heatRatio;
	this->pressure = pressure;
}

GasBody::~GasBody()
{
}
