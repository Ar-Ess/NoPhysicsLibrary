#include "DynamicBody.h"

DynamicBody::DynamicBody(Rect rect, Point velocity, Point gravityOffset, float mass, Flag* globals) : Body(BodyClass::DYNAMIC_BODY, rect, mass)
{
	this->velocity = velocity;
	this->gravityOffset = gravityOffset;
	this->globals = globals;
}

DynamicBody::~DynamicBody()
{
	if (!acousticDataList.empty())
	{
		for (AcousticData* data : acousticDataList) RELEASE(data);
	}
	acousticDataList.clear();
	forces.clear();
	momentums.clear();
}

void DynamicBody::Backup()
{
	backup = BodyBackup(rect.GetPosition(), velocity, acceleration, totalMomentum, totalForces);
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

	forces.emplace_back(new Force({ newtonsX, newtonsY }));
}

void DynamicBody::ApplyForce(Point newtons)
{
	if (globals->Get(0)) return; // Physics are paused
	if (!newtons.IsZero()) return; // If force is null

	forces.emplace_back(new Force({ newtons.x, newtons.y }));
}

void DynamicBody::ApplyMomentum(float momentumX, float momentumY)
{
	if (globals->Get(0)) return; // Physics are paused
	if (momentumX == 0 && momentumY == 0) return; // If momentum is null

	momentums.emplace_back(new Momentum({ momentumX, momentumY }));
}

void DynamicBody::ApplyMomentum(Point momentum)
{
	if (globals->Get(0)) return; // Physics are paused
	if (!momentum.IsZero()) return; // If momentum is null

	forces.emplace_back(new Force({ momentum.x, momentum.y }));
}

void DynamicBody::SetGravityOffset(Point gravityOffset)
{
	this->gravityOffset = gravityOffset;
}

inline Point DynamicBody::GetGravityOffset() const
{
	return gravityOffset;
}

void DynamicBody::SecondNewton()
{
	totalForces.Clear();

	for (Force* f : forces) totalForces.vector += f->vector;
	forces.clear();

	// You idiot, mass can not be zero :}
	assert(mass != 0);

	// total forces = kg * pxl / s^2

	// SUM Forces = massa * acceleració
	acceleration.x = totalForces.vector.x / mass;
	acceleration.y = totalForces.vector.y / mass;
}

void DynamicBody::FirstBuxeda()
{
	totalMomentum.Clear();

	for (Momentum* m : momentums) totalMomentum.vector += m->vector;
	momentums.clear();

	// You idiot, mass can not be zero :}
	assert(mass != 0);

	// SUM Forces = massa * acceleració
	velocity.x += totalMomentum.vector.x / mass;
	velocity.y += totalMomentum.vector.y / mass;
}