#include "StaticBody.h"

StaticBody::StaticBody(Rect rect, float mass, const float* pixelsToMeters) : Body(BodyClass::STATIC_BODY, rect, mass, pixelsToMeters)
{
}

StaticBody::~StaticBody()
{
}

void StaticBody::SetRestitutionOffset(Point offset)
{
	this->restitutionOffset = offset;
}
