#pragma once

#include "Flag.h"
#include "Collision.h"

class Physics
{
public: // Methods

	enum PhysicsGlobals
	{
		P_GLOBAL_PAUSE
	};

	virtual ~Physics();

	void Step(Body* b, float dt);

	void SolveCollisions(PhysArray<Body*>* bodies);

	void CleanUp();

private: // Methods

	Physics(const PhysArray<Body*>* bodies, const Flag* generalConfig, const PhysArray<unsigned int*>* gasIndex, const PhysArray<unsigned int*>* liquidIndex, const float* pixelsToMeters, const unsigned int* physIterations);

	void ApplyNaturalForces(Body* dynBody);
		void ApplyHydroForces(Body* dynBody);
			void ApplyHydroDrag(Body* dynBody, Body* env, const PhysRect insideRect);
			void ApplyHydroLift(Body* dynBody, Body* env);
			void  ApplyBuoyancy(Body* dynBody, Body* env, const float area);
		void ApplyAeroForces(Body* dynBody);
			void ApplyAeroDrag(Body* dynBody, Body* env, float area);
			void ApplyAeroLift(Body* dynBody, Body* env, float area);

	void Integrate(float dt, Body* body);

	// Declip
	void DetectCollisions(PhysArray<Body*>* bodies);
	void ResetFlags(PhysArray<Body*>* bodies);
	void Declip();

	// Internal
	PhysVec CalculateFriction(Body* body);
	bool IsCollisionDebuggingEnabled() const { return generalConfig->Get(0); }

public: // Variables

	PhysVec globalGravity = {0.0f, 0.0f };
	PhysVec globalRestitution = {1.0f, 1.0f };
	PhysVec globalFriction = {1.0f, 1.0f };

	// Pause 0
	Flag globals = {};

	PhysArray<Collision*> collisions;
	const PhysArray<unsigned int*>* gasIndex;
	const PhysArray<unsigned int*>* liquidIndex;
	const PhysArray<Body*>* bodies;

	// Config
	const Flag* generalConfig = nullptr;
	const float* pixelsToMeters = nullptr;
	const unsigned int* physIterations = nullptr;

private:

	friend class NoPhysicsLibrary;

};