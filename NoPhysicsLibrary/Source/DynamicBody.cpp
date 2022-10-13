#include "DynamicBody.h"

DynamicBody::DynamicBody(Rect rect, Point gravityOffset, float mass, Flag* globals, const float* pixelsToMeters) : Body(BodyClass::DYNAMIC_BODY, rect, mass, pixelsToMeters)
{
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

void DynamicBody::ApplyForce(float newtonsX, float newtonsY, InUnit unit)
{
	if (globals->Get(0)) return; // Physics are paused
	Point newtons = { newtonsX, newtonsY };

	if (newtons.IsZero()) return; // If newtons is null

	newtons *= mass;

	forces.emplace_back(new Force(newtons, unit));
}

void DynamicBody::ApplyForce(Point newtons, InUnit unit)
{
	if (newtons.IsZero()) return; // If newtons is null

	newtons *= mass;

	forces.emplace_back(new Force(newtons, unit));
}

void DynamicBody::ApplyMomentum(float momentumX, float momentumY, InUnit unit)
{
	if (globals->Get(0)) return; // Physics are paused
	Point momentum = { momentumX, momentumY };

	if (momentum.IsZero()) return; // If momentum is null

	momentums.emplace_back(new Momentum(momentum, unit));
}

void DynamicBody::ApplyMomentum(Point momentum, InUnit unit)
{
	if (globals->Get(0)) return; // Physics are paused

	if (momentum.IsZero()) return; // If momentum is null

	momentums.emplace_back(new Momentum(momentum, unit));
}

void DynamicBody::SetGravityOffset(Point gravityOffset, InUnit unit)
{
	if (unit == InUnit::IN_PIXELS) gravityOffset *= *pixelsToMeters;
	this->gravityOffset = gravityOffset;
}

Point DynamicBody::GetGravityOffset(InUnit unit) const
{
	float conversion = 1;
	if (unit == InUnit::IN_PIXELS) conversion = (1 / *pixelsToMeters);

	return gravityOffset.Multiply(conversion);
}

void DynamicBody::SecondNewton()
{
	totalForces.Clear();

	for (Force* f : forces)
	{
		Point vector = f->vector;
		if (f->unit == InUnit::IN_PIXELS) vector *= *pixelsToMeters;

		totalForces.vector += vector;
	}
	forces.clear();

	// You idiot, mass can not be zero :}
	assert(mass != 0);

	// SUM Forces = massa * acceleració
	acceleration.x = totalForces.vector.x / mass;
	acceleration.y = totalForces.vector.y / mass;
}

void DynamicBody::FirstBuxeda()
{
	totalMomentum.Clear();

	for (Momentum* m : momentums)
	{
		Point vector = m->vector;
		if (m->unit == InUnit::IN_PIXELS) vector *= *pixelsToMeters;

		totalMomentum.vector += vector;
	}
	momentums.clear();

	// You idiot, mass can not be zero :}
	assert(mass != 0);

	// SUM Forces = massa * acceleració
	velocity.x += totalMomentum.vector.x / mass;
	velocity.y += totalMomentum.vector.y / mass;
}