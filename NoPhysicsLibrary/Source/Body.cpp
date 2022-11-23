#include "Body.h"
#include "DirectionEnum.h"
#include "DynamicBody.h"
#include "Define.h"
#include "MathUtils.h"

Body::Body(BodyClass clas, Rect rect, float mass, const float* pixelsToMeters)
{
	this->clas = clas;
	this->rect = rect;
	this->emissionPoint = rect.GetPosition(Align::CENTER) - rect.GetPosition(Align::TOP_LEFT);
	this->pixelsToMeters = pixelsToMeters;

	// You idiot, mass can not be zero :}
	assert(mass != 0);

	this->mass = mass;

	this->properties.Set(true, 0);

	id = reinterpret_cast<int>(this);

	properties.Set(0b00000001);
}

void Body::Play(int index, float decibels)
{
	if (decibels > 120) decibels = 120;
	if (decibels < 0) decibels = 0;

	acousticDataList.emplace_back(new AcousticData(index, rect.GetPosition().Apply(emissionPoint), decibels));
}

void Body::SetEmissionPoint(Point point, InUnit unit)
{
	//-TODO: Review emission point set logic. Needs to be an offset from top-left, not a position
	if (unit == InUnit::IN_PIXELS) point *= *pixelsToMeters;

	Rect emissionRect = { emissionPoint.Apply(-1.0f, -1.0f) * *pixelsToMeters, Point(2.0f, 2.0f) * *pixelsToMeters };
	emissionPoint = rect.GetPosition().Apply(point.Negative());

	if (MathUtils::CheckCollision(rect, emissionRect)) return;

	emissionPoint = rect.GetPosition() - MathUtils::ClosestRectIntersectionFromOutsidePoint(emissionPoint, rect);
}

void Body::SetEmissionPoint(Align alignment, Point offset, InUnit unit)
{
	if (unit == InUnit::IN_PIXELS) offset *= *pixelsToMeters;

	emissionPoint = rect.GetPosition().Apply(rect.GetPosition(alignment).Negative());
	if (offset.IsZero()) return;

	emissionPoint += offset;
	Rect emissionRect = { emissionPoint.Apply(-1.0f, -1.0f) * *pixelsToMeters, Point(2.0f, 2.0f) * *pixelsToMeters };
	if (MathUtils::CheckCollision(rect, emissionRect)) return;

	emissionPoint = rect.GetPosition() - MathUtils::ClosestRectIntersectionFromOutsidePoint(emissionPoint, rect);
}

void Body::SetEmissionPoint(Align alignment)
{
	emissionPoint = rect.GetPosition().Apply(rect.GetPosition(alignment).Negative());
}

Point Body::GetPosition(InUnit unit, Align align) const
{
	float conversion = 1;
	if (unit == InUnit::IN_PIXELS) conversion = (1 / *pixelsToMeters);
	return rect.GetPosition(align) * conversion;
}

Point Body::GetSize(InUnit unit) const
{
	float conversion = 1;
	if (unit == InUnit::IN_PIXELS) conversion = (1 / *pixelsToMeters);
	return rect.GetSize() * conversion;
}
