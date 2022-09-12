#include "Body.h"
#include "DirectionEnum.h"
#include "DynamicBody.h"
#include "Define.h"

Body::Body(BodyClass clas, Rect rect, float mass, Audio* audio)
{
	this->clas = clas;
	this->rect = rect;

	// You idiot, mass can not be zero :}
	assert(mass != 0);

	this->mass = mass;

	this->properties.Set(true, 0);

	id = reinterpret_cast<int>(this);

	this->audio = audio;
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
