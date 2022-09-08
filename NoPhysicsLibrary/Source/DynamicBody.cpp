#include "DynamicBody.h"

DynamicBody::DynamicBody(Rect rect, Point velocity, Point gravity, float mass, Flag* globals, Audio* audio) : Body(BodyClass::DYNAMIC_BODY, rect, mass, audio)
{
	this->velocity = velocity;
	this->gravity = gravity;
	this->globals = globals;
}

DynamicBody::~DynamicBody()
{
	forces.clear();
	forces.shrink_to_fit();
	momentums.clear();
	momentums.shrink_to_fit();
}

BodyBackup DynamicBody::Backup()
{
	return BodyBackup(rect, velocity, acceleration, totalMomentum, totalForces);
}

bool DynamicBody::IsColliding(CollideBool collision)
{
	return collisionFlags.Get((int)collision);
}

bool DynamicBody::IsColliding()
{
	return collisionFlags.IsAnyTrue();
}

void DynamicBody::ApplyForce(float newtonsX, float newtonsY)
{
	if (globals->Get(0)) return; // Physics are paused
	if (newtonsX == 0 && newtonsY == 0) return; // If force is null

	forces.push_back(new Force({ newtonsX, newtonsY })); 
}

void DynamicBody::ApplyForce(Point newtons)
{
	if (globals->Get(0)) return; // Physics are paused
	if (!newtons.IsZero()) return; // If force is null

	forces.push_back(new Force({ newtons.x, newtons.y }));
}

void DynamicBody::ApplyMomentum(float momentumX, float momentumY)
{
	if (globals->Get(0)) return; // Physics are paused
	if (momentumX == 0 && momentumY == 0) return; // If momentum is null

	momentums.push_back(new Momentum({ momentumX, momentumY }));
}

void DynamicBody::ApplyMomentun(Point momentum)
{
	if (globals->Get(0)) return; // Physics are paused
	if (!momentum.IsZero()) return; // If momentum is null

	forces.push_back(new Force({ momentum.x, momentum.y }));
}

void DynamicBody::SecondNewton()
{
	totalForces.Clear();

	for (Force* f : forces) totalForces.magnitude += f->magnitude;
	forces.clear();

	// You idiot, mass can not be zero :}
	assert(mass != 0);

	// SUM Forces = massa * acceleraci�
	acceleration.x = totalForces.magnitude.x / mass;
	acceleration.y = totalForces.magnitude.y / mass;
}

void DynamicBody::FirstBuxeda()
{
	totalMomentum.Clear();

	for (Momentum* m : momentums) totalMomentum.magnitude += m->magnitude;
	momentums.clear();

	// You idiot, mass can not be zero :}
	assert(mass != 0);

	// SUM Forces = massa * acceleraci�
	velocity.x += totalMomentum.magnitude.x / mass;
	velocity.y += totalMomentum.magnitude.y / mass;
}