#include "StaticBody.h"

StaticBody::StaticBody(PhysRect rect, float mass, const float* pixelsToMeters) : Body(BodyClass::STATIC_BODY, rect, mass, pixelsToMeters)
{
	SetMaterial(Material());
}

StaticBody::~StaticBody()
{
}

void StaticBody::RestitutionOffset(PhysVec offset)
{
	this->restitutionOffset = offset;
}

void StaticBody::SetMaterial(Material material)
{
	Body::SetMaterial(material);

	youngModulus = material.youngModulus;
}
