#pragma once

//#include "DynArray.h"
#include "Rect.h"
#include <vector>
#include "Body.h"

#define PIXELS_PER_METER 1.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 1.0f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((float) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define DEGREES_TO_RADIANS(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define RADIANS_TO_DEGREES(angleRadians) ((angleRadians) * 180.0 / M_PI)

#define REALITY_MULTIPLIER 60
#define NEWTONS_MULTIPLIER 100

//enum class PhysicsPreset
//{
//	NO_PRESET = -1,
//	PLATFORMER_PHYSICS_PRESET,
//	MOON_PHYSICS_PRESET,
//	SPACE_PHYSICS_PRESET
//};
//
//enum class ScenarioPreset
//{
//	NO_PRESET = -1,
//	LIMITS_1280x720_SCENARIO_PRESET,
//	PLATFORMER_1280x720_SCENARIO_PRESET,
//	WALLJUMP_1280x720_SCENARIO_PRESET,
//	SPHERE_1280x720_SCENARIO_PRESET,
//};
//
//enum class Direction
//{
//	NO_DIR = -1,
//	UP = 0,
//	DOWN,
//	LEFT,
//	RIGHT
//};
//
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
//
//struct GhostSlot
//{
//	GhostSlot() 
//	{
//		this->surface = 0;
//		this->dir = Direction::NO_DIR;
//		this->slot = -1;
//	}
//	GhostSlot(Direction direct, int surf, int slt)
//	{
//		dir = direct;
//		surface = surf;
//		slot = slt;
//	}
//	~GhostSlot()
//	{
//
//	}
//
//	int surface = 0;
//	Direction dir = Direction::NO_DIR;
//	int slot = -1;
//
//	bool operator >(const GhostSlot& a)
//	{
//		if (surface > a.surface) return true;
//		return false;
//	}
//	bool operator <(const GhostSlot& a)
//	{
//		if (surface < a.surface) return true;
//		return false;
//	}
//};

class Physics
{
public:

	Physics();

	virtual ~Physics();

	bool Start();

	//Update and step your physics
	bool Update(float dt);

	//Draw your physic bodies. First argument shows specific propierties from the given DynamicBody*
	/*void Draw(float dt, DynamicBody* dB = nullptr);*/

	bool CleanUp();

public:

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
	//Sets body as the main player, for specific functions of it. Returns false if there is already one body setted as player.
	bool SetBodyAsPlayer(Body* b);

public:
	bool debug = false;
	bool debugBools = false;

	//// Debug
	//void Step(float dt);
	//void Integrate(DynamicBody* item, float dt);
	//Body* CheckCollisions(Body* body, Point prevPos);
	//Direction DirectionDetection(Point currPos, Point prevPos);
	//Direction InvertDirection(Direction dir);
	//void ChangeGravityAcceleration(Point acceleration);
	//void ChangeRestitutionCoeficient(Point restCoeficient);
	//void ChangeFrictionCoeficient(Point frictCoeficient);
	//void DebugDraw();
	//void DebugBools(DynamicBody* dB = nullptr, float dt = 0.0f);
	//void ResetBodyBools();

	std::vector<Body*> bodies;

private:
	Point globalGravity = {};
	Point globalRestitution = {};
	Point globalFriction = {};
	bool globalGravityActive = false;
	bool globalRestitutionActive = false;
	bool globalFrictionActive = false;
	bool globalPause = false;
};