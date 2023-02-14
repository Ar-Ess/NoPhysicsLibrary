#pragma once

#include <vector>
#include "Body.h"
#include "Flag.h"
#include "Collision.h"

#define DEGREES_TO_RADIANS(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define RADIANS_TO_DEGREES(angleRadians) ((angleRadians) * 180.0 / M_PI)

class Physics
{
public: // Methods

	Physics(const std::vector<Body*>* bodies, const Flag* physicsConfig, const std::vector<unsigned int*>* gasIndex, const std::vector<unsigned int*>* liquidIndex, const float* pixelsToMeters, const unsigned int* physIterations);

	virtual ~Physics();

	void Step(Body* b, float dt);

	void SolveCollisions(std::vector<Body*>* bodies);

	void CleanUp();

private: // Methods

	void ApplyNaturalForces(DynamicBody* body);
		void ApplyHydroForces(DynamicBody* body);
			void ApplyHydroDrag(DynamicBody* body, Body* env);
			void ApplyHydroLift(DynamicBody* body, Body* env);
			void  ApplyBuoyancy(DynamicBody* body, Body* env, float area);
		void ApplyAeroForces(DynamicBody* body);
			void ApplyAeroDrag(DynamicBody* body, Body* env, float area);
			void ApplyAeroLift(DynamicBody* body, Body* env, float area);

	void Integrate(float dt, Body* body);

	// Declip
	void DetectCollisions(std::vector<Body*>* bodies);
	void ResetFlags(std::vector<Body*>* bodies);
	void Declip();

	// Internal
	PhysVec CalculateFriction(DynamicBody* body);

public: // Variables

	PhysVec globalGravity = {0.0f, 0.0f };
	PhysVec globalRestitution = {1.0f, 1.0f };
	PhysVec globalFriction = {1.0f, 1.0f };

	// Pause 0
	Flag globals = {};

	std::vector<Collision*> collisions;
	const std::vector<unsigned int*>* gasIndex;
	const std::vector<unsigned int*>* liquidIndex;
	const std::vector<Body*>* bodies;

	// Config
	const Flag* physicsConfig = nullptr;
	const float* pixelsToMeters = nullptr;
	const unsigned int* physIterations = nullptr;

};