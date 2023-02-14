#pragma once

#include "Body.h"
#include "BodyStateEnum.h"
#include "BodyBackup.h"
#include "PhysID.h"

class DynamicBody : public Body
{
public: // Methods

	DynamicBody(PhysRect rect, PhysVec gravityOffset, float mass, Flag* globals, const float* pixelsToMeters);

	~DynamicBody() override;

	// Returns if this body is colliding with any other body in the specified situation
	bool IsBodyEnter(BodyState collision);

	// Returns if this body is colliding with any other body in the specified situation
	bool IsBodyStill(BodyState collision);

	// Returns if this body is colliding with any other body in the specified situation
	bool IsBodyExit(BodyState collision);

	// Adds an specific body to a query to not process a possible collision with it.
	// Good Practice: Add all bodies with impossible collision with this one to improve physics engine performance
	void ExcludeForCollision(const Body* b);
	
	bool IncludeForCollision(const Body* b);
	
	// Applies an specific force to this body
	void ApplyForce(float x, float y, InUnit unit = InUnit::IN_METERS);
	// Applies an specific force to this body
	void ApplyForce(PhysVec force, InUnit unit = InUnit::IN_METERS);

	// Applies an specific force to this body
	void ApplyMomentum(float x, float y, InUnit unit = InUnit::IN_METERS);
	// Applies an specific force to this body
	void ApplyMomentum(PhysVec momentum, InUnit unit = InUnit::IN_METERS);

	// Sets a local gravity to the body
	void GravityOffset(PhysVec offset, InUnit unit = InUnit::IN_METERS);
	// Returns the local gravity vector of the body
	PhysVec GravityOffset(InUnit unit = InUnit::IN_METERS) const;

	// Sets a friction offset to the body
	// Value between 0 and 1. Out of bounds values will be clamped to 0 / 1.
	void FrictionOffset(PhysVec offset);
	// Returns the local gravity vector of the body
	// Value between 0 and 1
	PhysVec FrictionOffset() { return frictionOffset; }

	// Sets a restitution offset to the body
	void RestitutionOffset(PhysVec offset);
	// Returns the friction offset to the body
	PhysVec RestitutionOffset() { return restitutionOffset; }
	
	void ResetForces();

private: // Methods

	void SecondNewton();
	void FirstBuxeda();
	void Backup();
	bool IsIdExcludedFromCollision(PhysID id);
	void SetPreviousBodyState();

private: // Variables

	friend class Physics; // Necessary for accessing SecondNewton/FirstBuxeda
	friend class NPL; // Necessary to update states

	PhysVec acceleration = {};
	PhysVec velocity = {};
	PhysVec gravityOffset = {};
	PhysVec frictionOffset = {};
	PhysVec restitutionOffset = {};

	PhysArray<Force*> forces;
	PhysArray<Momentum*> momentums;
	Force totalForces = {};
	Momentum totalMomentum = {};

	// Gas | Ground | Roof | Left | Right | Liquid | Floating | Moving
	Flag bodyStateEnter = {};
	Flag bodyStateStay = {};
	Flag bodyStateExit = {};
	Flag prevBodyState = {};
	//-TODO: IsBodyExit (BodyStateExit). Basically unity's "on collision enter/stay/exit".

	Flag* globals = nullptr;

	BodyBackup backup = {};

	PhysArray<PhysID> excludeCollisionIds;
};
