#include "Body.h"
#include "DirectionEnum.h"
#include "DynamicBody.h"
#include "Define.h"
#include "PhysMath.h"
#include "PhysTrigger.h"

Body::Body(BodyClass clas, PhysRect rect, float mass, const float* pixelsToMeters)
{
	this->clas = clas;
	this->rect = rect;
	this->emissionPoint = 0;
	this->pixelsToMeters = pixelsToMeters;

	this->mass = mass;

	this->properties.Set(true, 0);

	id = new PhysID();

	properties.Set(0b00000111);
}

void Body::Play(unsigned int index, float decibels)
{
	PhysMath::Clamp(decibels, 0, 120);
	acousticDataList.Add(new AcousticData(index, decibels));
	!(*playSoundTrigger);
}

void Body::EmissionPoint(PhysVec offset, InUnit unit)
{
	emissionPoint = offset * Conversion(unit, true);
}

PhysVec Body::EmissionPoint(InUnit unit) const
{
	return (rect.Position() + (rect.Size() / 2) + emissionPoint) * Conversion(unit, false);
}

void Body::ReceptionPoint(PhysVec offset, InUnit unit)
{
	receptionPoint = offset * Conversion(unit, true);
}

PhysVec Body::ReceptionPoint(InUnit unit) const
{
	return (rect.Position() + (rect.Size() / 2) + receptionPoint) * Conversion(unit, false);
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

void Body::Size(float w, float h, InUnit unit)
{
	const float conv = Conversion(unit, true);
	rect.w = w * conv;
	rect.h = h * conv;
}

PhysRect Body::Rect(InUnit unit) const
{
	return { Position(unit), Size(unit) };
}
