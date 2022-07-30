#include "DynamicBody.h"

DynamicBody::DynamicBody(Rect rect, Point velocity, Point gravity, float mass) : Body(BodyClass::DYNAMIC_BODY, rect, mass)
{
	this->velocity = velocity;
	this->gravityAcceleration = gravity;
}

BodyBackUp DynamicBody::BackUp()
{
	return BodyBackUp(rect, velocity, acceleration, sumMomentum, sumForces);
}

void DynamicBody::SecondNewton()
{
	// TODO: Crear struct force, amb info del Point, el mòdul, qui l'ha aplicat, quin tipus (buoyancy, aerodrag...)

	sumForces = { 0.0f,0.0f };

	for (Point p : forces) sumForces += p;
	forces.clear();

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

	for (Point m : momentums) sumMomentum += m;
	momentums.clear();

	// You idiot, mass can not be zero :}
	assert(mass != 0);

	// SUM Momentum = massa * velocity
	velocity.x += sumMomentum.x / mass;
	velocity.y += sumMomentum.y / mass;
}