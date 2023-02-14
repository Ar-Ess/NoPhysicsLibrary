#include "StaticBody.h"

StaticBody::StaticBody(PhysRect rect, float mass, const float* pixelsToMeters) : Body(BodyClass::STATIC_BODY, rect, mass, pixelsToMeters)
{
}

StaticBody::~StaticBody()
{
}

void StaticBody::RestitutionOffset(PhysVec offset)
{
	this->restitutionOffset = offset;
}
