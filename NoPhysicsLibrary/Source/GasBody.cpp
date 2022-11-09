#include "GasBody.h"

GasBody::GasBody(Rect rect, float mass, float heatRatio, float pressure, Point dragCoefficient, const float* pixelsToMeters) : Body(BodyClass::GAS_BODY, rect, mass, pixelsToMeters)
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
