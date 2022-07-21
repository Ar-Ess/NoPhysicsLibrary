#pragma once

//#include "DynArray.h"
#include "Rect.h"
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

//enum class DashDirection
//{
//	UP = 0,
//	DOWN,
//	LEFT,
//	RIGHT,
//	UPLEFT,
//	DOWNLEFT,
//	UPRIGHT,
//	DOWNRIGHT
//};
//
//enum class DebugBools
//{
//	GROUND,
//	ROOF,
//	LEFT_WALL,
//	RIGHT_WALL,
//	JUMP,
//	DOUBLE_JUMP,
//	WALL_JUMP,
//	DASH
//};

class Physics
{
public: // Methods

	Physics();

	virtual ~Physics();

	void Update(float dt);

	//Draw your physic bodies. First argument shows specific propierties from the given DynamicBody*
	/*void Draw(float dt, DynamicBody* dB = nullptr);*/

	bool CleanUp();

	void SetGlobalGravity(Point gravity);
	//Sets global restitution coeficient for all bodies. {0.0f, 0.0f} for perfect elastic restitution
	void SetGlobalRestitution(Point restCoeficient);
	//Sets global friction for all bodies. {0.0f, 0.0f} for no friction
	void SetGlobalFriction(Point frictionCoeficient);
	////Sets a default physics presset
	//void SetPhysicsPreset(PhysicsPreset phPreset);
	////Sets a default scenario presset
	//void SetScenarioPreset(ScenarioPreset sPreset);
	//Kills a body when outside the rect inputted
	void DeathLimit(Rect limits);
	//Kills all static bodies in the screen
	void DestroyScenario();
	//Pauses all physics. Depending of true/false inputted, will pause/unpause the physics respectively
	void PausePhysics(bool pause);
	//Resets to 0 all forces, velocities, accelerations applied to any existing dynamic body
	void ResetAllForces();
	//Destroys de body setted as an input of this function
	void DestroyBody(std::vector<Body*>::const_iterator it);

public: // Getters & Setters

	// Returns wether the physics are paused
	inline bool GetGlobalPause() { return globals.Get(0); }

	// Returns wether the global gravity is active
	inline bool GetGlobalGravity() { return globals.Get(1); }

	// Returns wether the global friction is active
	inline bool GetGlobalFriction() { return globals.Get(2); }

	// Returns wether the global restitution is active
	inline bool GetGlobalRestitution() { return globals.Get(3); }

public:

	bool debug = false;
	bool debugBools = false;

	void Integrate(DynamicBody* item, float dt);
	void CheckCollisions(Body* body, Point prevPos);
	int DirectionDetection(Point currPos, Point prevPos);
	int InvertDirection(int dir);
	//void ChangeGravityAcceleration(Point acceleration);
	//void ChangeRestitutionCoeficient(Point restCoeficient);
	//void ChangeFrictionCoeficient(Point frictCoeficient);
	//void DebugDraw();
	//void DebugBools(DynamicBody* dB = nullptr, float dt = 0.0f);
	//void ResetBodyBools();

	std::vector<Body*> bodies;
	Point globalGravity = {};
	Point globalRestitution = {};
	Point globalFriction = {};

private:

	// Pause | Gravity | Friction | Restitution
	Flag globals = {};

};