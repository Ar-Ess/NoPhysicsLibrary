#include "LiquidBody.h"

LiquidBody::LiquidBody(Rect rect, float mass, const float* pixelsToMeters) : Body(BodyClass::LIQUID_BODY, rect, mass, pixelsToMeters)
{
}

LiquidBody::~LiquidBody()
{
}
