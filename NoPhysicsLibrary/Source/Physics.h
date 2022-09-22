#pragma once

#include <vector>
#include "Body.h"
#include "Flag.h"
#include "Collision.h"

#define PIXELS_PER_METER 1.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 1.0f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((float) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define DEGREES_TO_RADIANS(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define RADIANS_TO_DEGREES(angleRadians) ((angleRadians) * 180.0 / M_PI)

#define REALITY_MULTIPLIER 60
#define NEWTONS_MULTIPLIER 100

class Physics
{
public: // Methods

	Physics();

	virtual ~Physics();

	void Step(Body* body, float dt);

	void Declip(std::vector<Body*>* bodies);

	void CleanUp();

	bool CheckCollision(Rect rect1, Rect rect2);

private: // Methods

	void UpdateDynamic(float dt, Body* body);

	void UpdateLiquid(float dt, Body* body);

	void AutoApplyForces();

	void AutoApplyAeroDrag();

	void AutoApplyAeroLift();

	void AutoApplyHydroDrag();

	void AutoApplyHydroLift(); // Future idea, maybe implementable

	void AutoApplyBuoyancy();

	void Integrate(float dt, Body* body);
	/*void CheckCollisions(Body* body, BodyBackup backup);*/
	int DirectionDetection(Point currPos, Point prevPos);
	int InvertDirection(int dir);
	//void ChangeGravityAcceleration(Point acceleration);
	//void ChangeRestitutionCoeficient(Point restCoeficient);
	//void ChangeFrictionCoeficient(Point frictCoeficient);

public: // Variables

	Point globalGravity = {};
	Point globalRestitution = {};
	Point globalFriction = {};

	// Pause 0 | Restitution 1 |
	Flag globals = {};

	std::vector<Collision*> collisions;

};