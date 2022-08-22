#pragma once

//#include "DynArray.h"
#include "BodyBackUp.h"
#include <vector>
#include "DynamicBody.h"
#include "StaticBody.h"
#include "Flag.h"

#define PIXELS_PER_METER 1.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 1.0f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((float) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define DEGREES_TO_RADIANS(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define RADIANS_TO_DEGREES(angleRadians) ((angleRadians) * 180.0 / M_PI)

#define REALITY_MULTIPLIER 60
#define NEWTONS_MULTIPLIER 100

// Deletes a buffer
#define RELEASE( x ) \
	{						\
	if( x != NULL )		\
		{					  \
	  delete x;			\
	  x = NULL;			  \
		}					  \
	}

class Physics
{
public: // Methods

	Physics();

	virtual ~Physics();

	void Update(float dt);

	void CleanUp();

	bool DestroyBody(Body* body);

	bool CheckCollision(Rect rect1, Rect rect2);

private:

	void UpdateDynamic(float dt, DynamicBody* body);

	void UpdateLiquid(float dt);

	void AutoApplyForces();

	void AutoApplyAeroDrag();

	void AutoApplyAeroLift();

	void AutoApplyHydroDrag();

	void AutoApplyHydroLift(); // Future idea, maybe implementable

	void AutoApplyBuoyancy();

	bool EraseBody(Body* body);

public:

	void Integrate(DynamicBody* item, float dt);
	void CheckCollisions(Body* body, BodyBackUp backup);
	int DirectionDetection(Point currPos, Point prevPos);
	int InvertDirection(int dir);
	//void ChangeGravityAcceleration(Point acceleration);
	//void ChangeRestitutionCoeficient(Point restCoeficient);
	//void ChangeFrictionCoeficient(Point frictCoeficient);

	// -TODO: Investigate about vector. Maybe not the best option if users has to save a pointer to data
	// inside here. Vectors reorder themselves if they don't find enough space. And Idk if that could
	// lead to a "nullptr" error. Look video: https://www.youtube.com/watch?v=6OoSgY6NVVk&t=1550s&ab_channel=javidx9
	std::vector<Body*> bodies;

	Point globalGravity = {};
	Point globalRestitution = {};
	Point globalFriction = {};

	// Pause 0
	Flag globals = {};

};