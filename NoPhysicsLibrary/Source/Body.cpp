#include "Body.h"
#include "DirectionEnum.h"
#include "DynamicBody.h"

Body::Body(BodyClass clas, Rect rect, float mass)
{
	this->clas = clas;
	this->rect = rect;

	if (mass <= 0.0f) mass = 1.0f;
	this->mass = mass;

	this->properties.Set(true, 0);
}

void Body::SolveCollision(Body& body, int dir)
{
	if (!body.IsCollidable()) return;
	DeClipper(body, dir); // First declip, then do anything 
}

void Body::DeClipper(Body &body, int dir)
{
	Direction direction = (Direction)dir;
	
	switch (clas)
	{
	case BodyClass::DYNAMIC_BODY:
		DynamicBody* currentBody = (DynamicBody*)this;

			//TOP & DOWN & LEFT & RIGHT
			if ((currentBody->rect.y + currentBody->rect.h > body.rect.y) && (currentBody->rect.y < body.rect.y) && (currentBody->rect.y + currentBody->rect.h < body.rect.y + body.rect.h))
			{
				// Ground
				currentBody->rect.y = body.rect.y - currentBody->rect.h;
				currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;

				currentBody->onGround = true;
				currentBody->dashCount = 0;
				if (currentBody->onJump)
				{
					currentBody->onJump = false;
					if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
				}
				if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
				if (currentBody->onWallJump) currentBody->onWallJump = false;
				if (currentBody->onDash) currentBody->onDash = false;
			}
			else if ((currentBody->rect.y < body.rect.y + body.rect.h) && (currentBody->rect.y + currentBody->rect.h > body.rect.y + body.rect.h) && (currentBody->rect.y > body.rect.y))
			{
				// Top
				currentBody->rect.y = body.rect.y + body.rect.h;
				currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;

				currentBody->onRoof = true;
			}
			else if ((currentBody->rect.x < body.rect.x + body.rect.w) && (currentBody->rect.x > body.rect.x) && (currentBody->rect.x + currentBody->rect.w > body.rect.x + body.rect.w))
			{
				// Left wall
				currentBody->rect.x = body.rect.x + body.rect.w;
				currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
				currentBody->onLeftWall = true;

				if (currentBody->onWallJump) currentBody->onWallJump = false;
			}
			else if ((currentBody->rect.x + currentBody->rect.w > body.rect.x) && (currentBody->rect.x + currentBody->rect.w < body.rect.x + body.rect.w) && (currentBody->rect.x < body.rect.x))
			{
				// Right wall
				currentBody->rect.x = body.rect.x - currentBody->rect.w;
				currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
				currentBody->onRightWall = true;

				if (currentBody->onWallJump) currentBody->onWallJump = false;
			}

		break;
	}
}
