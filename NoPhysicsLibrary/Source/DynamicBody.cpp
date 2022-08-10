#include "DynamicBody.h"

DynamicBody::DynamicBody(Rect rect, Point velocity, Point gravity, float mass) : Body(BodyClass::DYNAMIC_BODY, rect, mass)
{
	this->velocity = velocity;
	this->gravity = gravity;
}

BodyBackUp DynamicBody::BackUp()
{
	return BodyBackUp(rect, velocity, acceleration, totalMomentum, totalForces);
}

bool DynamicBody::IsColliding(CollideBool collision)
{
	return collisionFlags.Get((int)collision);
}

bool DynamicBody::IsColliding()
{
	return collisionFlags.IsAnyTrue();
}

void DynamicBody::SecondNewton()
{
	// TODONE: Crear struct force, amb info del Point, el mòdul, qui l'ha aplicat, quin tipus (buoyancy, aerodrag...)

	totalForces.Clear();

	for (Force* f : forces) totalForces.magnitude += f->magnitude;
	forces.clear();
	forces.shrink_to_fit();

	// You idiot, mass can not be zero :}
	assert(mass != 0);

	// SUM Forces = massa * acceleració
	acceleration.x = totalForces.magnitude.x / mass;
	acceleration.y = totalForces.magnitude.y / mass;
}

void DynamicBody::FirstBuxeda()
{
	// TODONE: Crear struct momentum, amb info del Point, el mòdul, qui l'ha aplicat, quin tipus (buoyancy, aerodrag...)

	totalMomentum.Clear();

	for (Momentum* m : momentums) totalMomentum.magnitude += m->magnitude;
	momentums.clear();
	momentums.shrink_to_fit();

	// You idiot, mass can not be zero :}
	assert(mass != 0);

	// SUM Forces = massa * acceleració
	velocity.x = totalMomentum.magnitude.x / mass;
	velocity.y = totalMomentum.magnitude.y / mass;
}