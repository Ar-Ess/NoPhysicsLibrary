#include "DynamicBody.h"

DynamicBody::DynamicBody(Rect rect, Point velocity, Point gravity, float mass) : Body(BodyClass::DYNAMIC_BODY, rect, mass)
{
	this->velocity = velocity;
	this->gravityAcceleration = gravity;
}