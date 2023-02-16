#include "Body.h"
#include "DirectionEnum.h"
#include "DynamicBody.h"
#include "Define.h"
#include "PhysMath.h"

Body::Body(BodyClass clas, PhysRect rect, float mass, const float* pixelsToMeters)
{
	this->clas = clas;
	this->rect = rect;
	this->emissionPoint = 0;
	this->pixelsToMeters = pixelsToMeters;

	// You idiot, mass can not be zero :}
	//-Todo: This must be enabled
	//assert(mass != 0);

	this->mass = mass;

	this->properties.Set(true, 0);

	id = PhysID();

	properties.Set(0b00000111);
}

void Body::Play(unsigned int index, float decibels)
{
	PhysMath::Clamp(decibels, 0, 120);
	acousticDataList.Add(new AcousticData(index, rect.Position() + emissionPoint, decibels));
}

void Body::EmissionPoint(PhysVec offset, InUnit unit)
{
	emissionPoint = offset * Conversion(unit, true);
}

PhysVec Body::EmissionPoint(InUnit unit) const
{
	return (rect.Position() + (rect.Size() / 2) + emissionPoint) * Conversion(unit, false);
}

PhysVec Body::Position(InUnit unit) const
{
	return rect.Position() * Conversion(unit, false);
}

void Body::Position(float x, float y, InUnit unit)
{
	rect.Position(PhysVec(x, y) * Conversion(unit, true));
}

void Body::Position(PhysVec position, InUnit unit)
{
	rect.Position(position * Conversion(unit, true));
}

PhysVec Body::Size(InUnit unit) const
{
	float conversion = 1;
	if (unit == InUnit::IN_PIXELS) conversion = (1 / *pixelsToMeters);
	return rect.Size() * conversion;
}

PhysRect Body::Rect(InUnit unit) const
{
	return { Position(unit), Size(unit) };
}
