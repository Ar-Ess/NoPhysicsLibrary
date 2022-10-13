#include "StaticBody.h"

StaticBody::StaticBody(Rect rect, float mass, Flag* bodiesConfig, const float* pixelsToMeters) : Body(BodyClass::STATIC_BODY, rect, mass, bodiesConfig, pixelsToMeters)
{
}

StaticBody::~StaticBody()
{
}
