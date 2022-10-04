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

	Physics(const Flag* physicsConfig, const float* pixelsToMeters);

	virtual ~Physics();

	void Step(Body* body, float dt);

	void SolveCollisions(std::vector<Body*>* bodies);

	void CleanUp();

private: // Methods

	// Step
	void UpdateDynamic(float dt, Body* body);
	void UpdateLiquid(float dt, Body* body);

	void AutoApplyForces();
	void AutoApplyAeroDrag();
	void AutoApplyAeroLift();
	void AutoApplyHydroDrag();
	void AutoApplyHydroLift(); // Future idea, maybe implementable
	void AutoApplyBuoyancy();

	void Integrate(float dt, Body* body);

	// Declip
	void DetectCollisions(std::vector<Body*>* bodies);
	void Declip();



public: // Variables

	Point globalGravity = {};
	Point globalRestitution = {};
	Point globalFriction = {};

	// Pause 0 | Restitution 1 |
	Flag globals = {};

	std::vector<Collision*> collisions;

	// Config
	const Flag* physicsConfig = {};
	const float* pixelsToMeters = nullptr;

};