#include "LiquidBody.h"

LiquidBody::LiquidBody(Rect rect, float mass, Flag* bodiesConfig, const float* pixelsToMeters) : Body(BodyClass::LIQUID_BODY, rect, mass, bodiesConfig, pixelsToMeters)
{
}

LiquidBody::~LiquidBody()
{
}
