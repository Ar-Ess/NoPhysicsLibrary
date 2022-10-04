#include "Body.h"
#include "DirectionEnum.h"
#include "DynamicBody.h"
#include "Define.h"
#include "MathUtils.h"

Body::Body(BodyClass clas, Rect rect, float mass, const float* pixelsToMeters)
{
	this->clas = clas;
	this->rect = rect;
	this->emissionPoint = rect.GetPosition(Align::CENTER);
	this->pixelsToMeters = pixelsToMeters;

	// You idiot, mass can not be zero :}
	assert(mass != 0);

	this->mass = mass;

	this->properties.Set(true, 0);

	id = reinterpret_cast<int>(this);
}

void Body::SetEmissionPoint(Point point, InUnit unit)
{
	if (unit == InUnit::IN_PIXELS) point *= *pixelsToMeters;

	Rect emissionRect = {point.Apply(-1, -1), 2, 2};
	emissionPoint = point;
	if (MathUtils::CheckCollision(rect, emissionRect)) return;

	emissionPoint = MathUtils::ClosestRectIntersectionFromOutsidePoint(emissionPoint, rect);
}

void Body::SetEmissionPoint(Align alignment, Point offset, InUnit unit)
{
	if (unit == InUnit::IN_PIXELS) offset *= *pixelsToMeters;

	emissionPoint = rect.GetPosition(alignment);
	if (offset.IsZero()) return;

	emissionPoint += offset;
	Rect emissionRect = { emissionPoint, 2, 2 };
	if (MathUtils::CheckCollision(rect, emissionRect)) return;

	emissionPoint = MathUtils::ClosestRectIntersectionFromOutsidePoint(emissionPoint, rect);
}

Point Body::GetPosition(InUnit unit) const
{
	float conversion = 1;
	if (unit == InUnit::IN_PIXELS) conversion = (1 / *pixelsToMeters);
	return rect.GetPosition() * conversion;
}

Point Body::GetSize(InUnit unit) const
{
	float conversion = 1;
	if (unit == InUnit::IN_PIXELS) conversion = (1 / *pixelsToMeters);
	return rect.GetSize() * conversion;
}

void Body::SolveCollision(Body& body, int dir)
{
	if (!body.IsCollidable()) return;
	DeClipper(body, dir); // First declip, then do anything 
}

void Body::DeClipper(Body &body, int dir)
{
	//Direction direction = (Direction)dir;
	//
	//switch (clas)
	//{
	//case BodyClass::DYNAMIC_BODY:
	//	DynamicBody* currentBody = (DynamicBody*)this;

	//		//TOP & DOWN & LEFT & RIGHT
	//		if ((currentBody->rect.y + currentBody->rect.h > body.rect.y) && (currentBody->rect.y < body.rect.y) && (currentBody->rect.y + currentBody->rect.h < body.rect.y + body.rect.h))
	//		{
	//			// Ground
	//			currentBody->rect.y = body.rect.y - currentBody->rect.h;
	//			currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;

	//			currentBody->collisionFlags.Set(true, INT_TO_BOOL((int)CollideBool::GROUND));
	//		}
	//		else if ((currentBody->rect.y < body.rect.y + body.rect.h) && (currentBody->rect.y + currentBody->rect.h > body.rect.y + body.rect.h) && (currentBody->rect.y > body.rect.y))
	//		{
	//			// Top
	//			currentBody->rect.y = body.rect.y + body.rect.h;
	//			currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;

	//			currentBody->collisionFlags.Set(true, INT_TO_BOOL((int)CollideBool::ROOF));
	//		}
	//		else if ((currentBody->rect.x < body.rect.x + body.rect.w) && (currentBody->rect.x > body.rect.x) && (currentBody->rect.x + currentBody->rect.w > body.rect.x + body.rect.w))
	//		{
	//			// Left wall
	//			currentBody->rect.x = body.rect.x + body.rect.w;
	//			currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
	//			
	//			currentBody->collisionFlags.Set(true, INT_TO_BOOL((int)CollideBool::LEFT));
	//		}
	//		else if ((currentBody->rect.x + currentBody->rect.w > body.rect.x) && (currentBody->rect.x + currentBody->rect.w < body.rect.x + body.rect.w) && (currentBody->rect.x < body.rect.x))
	//		{
	//			// Right wall
	//			currentBody->rect.x = body.rect.x - currentBody->rect.w;
	//			currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
	//			
	//			currentBody->collisionFlags.Set(true, INT_TO_BOOL((int)CollideBool::RIGHT));
	//		}

	//	break;
	//}
}
