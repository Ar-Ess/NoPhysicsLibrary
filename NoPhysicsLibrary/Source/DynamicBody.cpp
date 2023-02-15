#include "DynamicBody.h"
#include "PhysMath.h"

DynamicBody::DynamicBody(PhysRect rect, PhysVec gravityOffset, float mass, Flag* globals, const float* pixelsToMeters) : 
	Body(BodyClass::DYNAMIC_BODY, rect, mass, pixelsToMeters)
{
	this->gravityOffset = gravityOffset;
	this->globals = globals;
}

DynamicBody::~DynamicBody()
{
	acousticDataList.Clear();
	forces.Clear();
	momentums.Clear();
	excludeCollisionIds.Clear();
}

void DynamicBody::Backup()
{
	backup = BodyBackup(rect, velocity, acceleration, totalMomentum, totalForces);
}

bool DynamicBody::IsIdExcludedFromCollision(PhysID id)
{
	return excludeCollisionIds.Contains(id);
}

void DynamicBody::SetPreviousBodyState()
{
	prevBodyState.Set(bodyStateStay.Binary());
}

bool DynamicBody::IsBodyEnter(BodyState collision)
{
	return bodyStateEnter.Get((int)collision);
}

bool DynamicBody::IsBodyStill(BodyState collision)
{
	return bodyStateStay.Get((int)collision);
}

bool DynamicBody::IsBodyExit(BodyState collision)
{
	return bodyStateExit.Get((int)collision);
}

void DynamicBody::ExcludeForCollision(const Body* b)
{
	excludeCollisionIds.Add(b->Id());
}

bool DynamicBody::IncludeForCollision(const Body* b)
{
	int index = excludeCollisionIds.Find(b->Id());
	if (index == -1) return false;

	excludeCollisionIds.Erase(index);

	return false;
}

void DynamicBody::ApplyForce(float x, float y, InUnit unit)
{
	if (globals->Get(0)) return; // Physics are paused
	PhysVec force = { x, y };

	if (force.IsZero()) return; // If forces is null
	force *= Conversion(unit, true);

	forces.Add(new Force(force));
}

void DynamicBody::ApplyForce(PhysVec force, InUnit unit)
{
	if (globals->Get(0)) return; // Physics are paused
	if (force.IsZero()) return; // If forces is null

	//-TODO: Check if it has to be times = 2 because of m^2
	force *= Conversion(unit, true);

	forces.Add(new Force(force));
}

void DynamicBody::ApplyMomentum(float momentumX, float momentumY, InUnit unit)
{
	if (globals->Get(0)) return; // Physics are paused
	PhysVec momentum = { momentumX, momentumY };

	if (momentum.IsZero()) return; // If momentum is null
	momentum *= Conversion(unit, true);

	momentums.Add(new Momentum(momentum));
}

void DynamicBody::ApplyMomentum(PhysVec momentum, InUnit unit)
{
	if (globals->Get(0)) return; // Physics are paused

	if (momentum.IsZero()) return; // If momentum is null
	momentum *= Conversion(unit, true);

	momentums.Add(new Momentum(momentum));
}

void DynamicBody::GravityOffset(PhysVec offset, InUnit unit)
{
	offset *= Conversion(unit, true);
	this->gravityOffset = offset;
}

PhysVec DynamicBody::GravityOffset(InUnit unit) const
{
	return gravityOffset * Conversion(unit, false);
}

void DynamicBody::SecondNewton()
{
	totalForces.Clear();

	forces.Iterate<Force&>
	(
		[](Force* f, Force& total)
		{
			total += *f;
		},
		totalForces
	);
	/*for(unsigned int i = 0; i < forces.Size(); ++i) 
		totalForces += *forces[i];*/

	forces.Clear();

	// You idiot, mass can not be zero :}
	if (mass == 0) return acceleration.Clear();

	// SUM Forces = massa * acceleració
	acceleration.x = totalForces.x / mass;
	acceleration.y = totalForces.y / mass;
}

void DynamicBody::FirstBuxeda()
{
	totalMomentum.Clear();

	forces.Iterate<Momentum&>
	(
		[](Momentum* m, Momentum& total)
		{
			total += *m;
		},
		totalMomentum
	);
	/*for(unsigned int i = 0; i < momentums.Size(); ++i) 
		totalMomentum += *momentums[i];*/
	
	momentums.Clear();

	// You idiot, mass can not be zero :}
	if (mass == 0) return;

	// SUM Forces = massa * acceleració
	velocity.x += totalMomentum.x / mass;
	velocity.y += totalMomentum.y / mass;
}

void DynamicBody::ResetForces()
{
	acceleration.Clear();
	velocity.Clear();
	forces.Clear();
	momentums.Clear();
}

void DynamicBody::FrictionOffset(PhysVec offset)
{
	PhysMath::Clamp(offset, 0, 1);
	this->frictionOffset = offset;
}

void DynamicBody::RestitutionOffset(PhysVec offset)
{
	this->restitutionOffset = offset;
}
