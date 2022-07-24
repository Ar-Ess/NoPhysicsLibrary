#include "DynamicBody.h"

DynamicBody::DynamicBody(Rect rect, Point velocity, Point gravity, float mass) : Body(BodyClass::DYNAMIC_BODY, rect, mass)
{
	this->velocity = velocity;
	this->gravityAcceleration = gravity;
}

void DynamicBody::SecondNewton()
{
	// TODO: Crear struct force, amb info del Point, el mòdul, qui l'ha aplicat, quin tipus (buoyancy, aerodrag...)

	sumForces = { 0.0f,0.0f };

	for (int i = 0; i < forces.Count(); i++) sumForces += *forces.At(i);
	forces.Clear();

	// You idiot, mass can not be zero :}
	assert(mass != 0);

	// SUM Forces = massa * acceleració
	acceleration.x = sumForces.x / mass;
	acceleration.y = sumForces.y / mass;
}

void DynamicBody::FirstBuxeda()
{
	// TODO: Crear struct momentum, amb info del Point, el mòdul, qui l'ha aplicat, quin tipus (buoyancy, aerodrag...)

	sumMomentum = { 0.0f,0.0f };

	for (int i = 0; i < momentums.Count(); i++) sumMomentum += *momentums.At(i);
	momentums.Clear();

	// You idiot, mass can not be zero :}
	assert(mass != 0);

	// SUM Momentum = massa * velocity
	velocity.x += sumMomentum.x / mass;
	velocity.y += sumMomentum.y / mass;
}