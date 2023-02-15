#pragma once

#include "Flag.h"
#include "Collision.h"

class Physics
{
public: // Methods

	Physics(const PhysArray<Body*>* bodies, const Flag* physicsConfig, const PhysArray<unsigned int*>* gasIndex, const PhysArray<unsigned int*>* liquidIndex, const float* pixelsToMeters, const unsigned int* physIterations);

	virtual ~Physics();

	void Step(Body* b, float dt);

	void SolveCollisions(PhysArray<Body*>* bodies);

	void CleanUp();

private: // Methods

	void ApplyNaturalForces(Body* dynBody);
		void ApplyHydroForces(Body* dynBody);
			void ApplyHydroDrag(Body* dynBody, Body* env);
			void ApplyHydroLift(Body* dynBody, Body* env);
			void  ApplyBuoyancy(Body* dynBody, Body* env, float area);
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
	const Flag* physicsConfig = nullptr;
	const float* pixelsToMeters = nullptr;
	const unsigned int* physIterations = nullptr;

};