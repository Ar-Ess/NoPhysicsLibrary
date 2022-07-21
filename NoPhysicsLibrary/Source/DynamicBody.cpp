#include "DynamicBody.h"

DynamicBody::DynamicBody(Rect rect, Point velocity, Point gravity, float mass) : Body(BodyClass::DYNAMIC_BODY, rect, mass)
{
	this->velocity = velocity;
	this->gravityAcceleration = gravity;
}

void DynamicBody::SecondNewton()
{
	for (int i = 0; i < forces.Count(); i++)
	{
		sumForces += *forces.At(i);
		forces.Pop(*forces.At(i));
	}
	forces.Clear();

	acceleration.x += sumForces.x / mass;
	acceleration.y += sumForces.y / mass;

	sumForces = { 0.0f,0.0f };
}

void DynamicBody::FirstBuxeda()
{
	for (int i = 0; i < momentums.Count(); i++)
	{
		sumMomentum += *momentums.At(i);
		momentums.Pop(*momentums.At(i));
	}
	momentums.Clear();

	velocity.x += sumMomentum.x / mass;
	velocity.y += sumMomentum.y / mass;

	sumMomentum = { 0.0f,0.0f };
}
