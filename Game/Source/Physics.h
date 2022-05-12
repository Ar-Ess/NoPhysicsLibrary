#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Collision.h"
#include "Log.h"
#include "Point.h"
#include "DynArray.h"
#include "Render.h"
#include "GuiManager.h"
#include <vector>

#define PIXELS_PER_METER 1.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 1.0f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define DEGREES_TO_RADIANS(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define RADIANS_TO_DEGREES(angleRadians) ((angleRadians) * 180.0 / M_PI)

#define REALITY_MULTIPLIER 60
#define NEWTONS_MULTIPLIER 100

struct SDL_Texture;
class Collision;
class GuiString;

enum BodyType
{
	NONE = -1,
	STATIC_BODY,
	DYNAMIC_BODY
};

enum CollisionType
{
	UNDEFINED = -1,
	RECTANGLE,
	CIRCLE
};

enum class PhysicsPreset
{
	NO_PRESET = -1,
	PLATFORMER_PHYSICS_PRESET,
	MOON_PHYSICS_PRESET,
	SPACE_PHYSICS_PRESET
};

enum class ScenarioPreset
{
	NO_PRESET = -1,
	LIMITS_1280x720_SCENARIO_PRESET,
	PLATFORMER_1280x720_SCENARIO_PRESET,
	WALLJUMP_1280x720_SCENARIO_PRESET,
	SPHERE_1280x720_SCENARIO_PRESET,
};

enum class Direction
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
};

enum class DashDirection
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	UPLEFT,
	DOWNLEFT,
	UPRIGHT,
	DOWNRIGHT
};

enum class DebugBools
{
	GROUND,
	ROOF,
	LEFT_WALL,
	RIGHT_WALL,
	JUMP,
	DOUBLE_JUMP,
	WALL_JUMP,
	DASH
};

struct GhostSlot
{
	GhostSlot() 
	{
		int surface = 0;
		Direction dir;
		int slot = -1;
	}
	GhostSlot(Direction direct, int surf, int slt)
	{
		dir = direct;
		surface = surf;
		slot = slt;
	}
	~GhostSlot()
	{

	}

	int surface = 0;
	Direction dir;
	int slot = -1;

	bool operator >(const GhostSlot& a)
	{
		if (surface > a.surface) return true;
		return false;
	}
	bool operator <(const GhostSlot& a)
	{
		if (surface < a.surface) return true;
		return false;
	}
};

class Body
{
public:
	Body(BodyType bodyType_, int a) :bodyType(bodyType_), texture(NULL), rect({}), circle({}) {} // Default constructor
	Body(BodyType bodyType_, CollisionType colliderType_ = CollisionType::UNDEFINED, Point position_ = { 0.0f, 0.0f }, Rect rect_ = {}, float mass_ = 1.0f) // Constructor with body type and collider type
	{
		bodyType = bodyType_;
		position = position_;
		colliderType = colliderType_;
		rect = rect_;
		mass = mass_;
	}
	Body(BodyType bodyType_, CollisionType colliderType_ = CollisionType::UNDEFINED, Point position_ = { 0.0f, 0.0f }, CircleCollider circle_ = {}, float mass_ = 1.0f) // Constructor with body type and collider type
	{
		bodyType = bodyType_;
		position = position_;
		colliderType = colliderType_;
		circle = circle_;
		rect = {};
		mass = mass_;
	}

public: //Getters

	// This function returns position x & y of the body
	Point GetPosition() const
	{
		return position;
	}
	// This function returns the width & height of the body (Rectangle) or the radius (Circle)
	Point GetMagnitudes() const
	{
		if (colliderType == RECTANGLE) return {rect.w, rect.h};
		return {(int)circle.radius, 0};
	}
	// This function returns the rect or circle of the body
	void GetCollision(Rect& rectangle)
	{
		rectangle = rect;
	}
	// This function returns the rect or circle of the body
	void GetCollision(CircleCollider& circ)
	{
		circ = circle;
	}
	// This function returns the type of collision the body has
	CollisionType GetCollisionType() const
	{
		return colliderType;
	}
	// This function returns the type of body it is
	BodyType GetBodyType() const
	{
		return bodyType;
	}
	// This function returns the mass of the body
	float GetMass() const
	{
		return mass;
	}
	//This function returns the body as an SDL_Rect (from SDL library) in case of being a Rectangle. If it is not, it returns {0, 0, 0, 0}.
	Rect ReturnBodyRect() const
	{
		if (colliderType == RECTANGLE) return rect;

		return {0, 0, 0, 0};
	}
	//This function changes the state of collision of a body. If true inputed, body will have collision, and viceversa
	void SetCollidable(bool collidable)
	{
		isCollidable = collidable;
	}

protected:
	friend class Physics;
	friend class Player;

	void SolveCollision(Body& body, Direction dir);
	void DeClipper(Body& body, Direction dir);
	double ToPositiveAngle(double angle);

	Rect rect;
	CircleCollider circle;
	SDL_Texture* texture = nullptr;

	BodyType bodyType;
	CollisionType colliderType;

	Point position;
	float rotation;
	float mass;
	bool isCollidable = true;
	bool player = false;
};

class StaticBody : public Body
{
public:
	StaticBody() :Body(BodyType::STATIC_BODY, 0) {}
	StaticBody(Point position_, CollisionType colliderType_, Rect rect_, uint mass_) :Body(BodyType::STATIC_BODY, colliderType_, position_, rect_, mass_) {}
	StaticBody(Point position_, CollisionType colliderType_, CircleCollider circle_, uint mass_) :Body(BodyType::STATIC_BODY, colliderType_, position_, circle_, mass_) {}
};

class DynamicBody : public Body
{
public:
	DynamicBody() :Body(BodyType::DYNAMIC_BODY, 0) {}
	DynamicBody(Point position, Point velocity, Point gravity, CollisionType colliderType, Rect rect, float mass) :Body(BodyType::DYNAMIC_BODY, colliderType, position, rect, mass)
	{
		this->velocity = velocity;
		this->gravityAcceleration = gravity;
	}
	DynamicBody(Point position, Point velocity, Point gravity, CollisionType colliderType, CircleCollider circle, float mass) :Body(BodyType::DYNAMIC_BODY, colliderType, position, circle, mass)
	{
		this->velocity = velocity;
		this->gravityAcceleration = gravity;
	}

	// Set an specific gravity for this body. Not compatible with SetGlobalGravity() being active. Desable SetGlobalgravity() by inputting SetGlobalGravity(0.0f, 0.0f); or deleting the function
	void SetGravityAcceleration(Point& gravity);
	// Set an constant velocity for this body.
	void SetConstantVelocity(Point& constVelocity);
	// Allow body to move given an specific force. It needs an input conditional ("if (LEFT ARROW REPEATED) Move(...)"). It allows to limit the velocity. If {0.0f, 0.0f} is inputted, velocity limit will not occur
	void Move(float newtons = 5.0f, Direction dir = Direction::LEFT, float velocityLimiter = 0.0f);
	// Allow body to dash given an specific force. It needs an input conditional ("if (LEFT ARROW DOWN) Dash(...)"). It only works on Air. "dashLimit" argument sets how many times can a body dash on air. If limit = 0, dash limit will not occur
	void Dash(float newtons = 400.0f, DashDirection dDir = DashDirection::LEFT, uint dashLimit = 0);
	// Allow body to jump given an specific force. It needs an input conditional ("if (SPACE BAR PRESSED) Jump(...)"). Input true/false for double jump ability
	void Jump(float newtonsY = 300.0f, bool doubleJump = false);
	// Allow body to jump from a wall given an specific force. It needs an input conditional ("if (SPACE BAR PRESSED) WallJump(...)"). Input true/false for wall friction descendence
	void WallJump(Point newtons = {200.0f, 200.0f}, bool wallFriction = false);
	// Applies an specific force to this body
	void ApplyForce(Point newtons);
	// Applies an specific force to this body
	void ApplyForce(float newtonsX = 0, float newtonsY = 0);
	// Applies an specific force to this body
	void ApplyMomentum(Point momentum);
	// Applies an specific force to this body
	void ApplyMomentum(float momentumX, float momentumY);
	//This function returns a debug bool by inputting which one you want to receive
	bool GetDebugBools(DebugBools dBool) const
	{
		bool boolSelec = false;

		switch (dBool)
		{
		case DebugBools::GROUND: boolSelec = onGround; break;
		case DebugBools::ROOF: boolSelec = onRoof; break;
		case DebugBools::LEFT_WALL: boolSelec = onLeftWall; break;
		case DebugBools::RIGHT_WALL: boolSelec = onRightWall; break;
		case DebugBools::JUMP: boolSelec = onJump; break;
		case DebugBools::DOUBLE_JUMP: boolSelec = onDoubleJump; break;
		case DebugBools::WALL_JUMP: boolSelec = onWallJump; break;
		case DebugBools::DASH: boolSelec = onDash; break;
		}

		return boolSelec;
	}
	// Restart the accelerations/velocities applied to the body
	void ResetForces();
	// Change the position of the body
	void SetPosition(Point newPosition)
	{
		position = newPosition;
		rect.x = newPosition.x;
		rect.y = newPosition.y;
	}

private:

	friend class Physics;
	friend class Player;

	void SecondNewton();
	void FirstBuxeda();
	void ApplyFriction(float dt);
	void ApplyBuoyancy();
	void ApplyHidroDrag();
	void ResetBools();

private:

	friend class Body;
	friend class Physics;
	Point velocity;
	Point gravityAcceleration;
	Point acceleration = {};
	Point sumForces = { 0.0f, 0.0f };
	Point sumMomentum = {0.0f, 0.0f};
	Point coeficientRestitution = { 1.0f,1.0f };
	Point frictionCoeficient = { 0.0f,0.0f };
	float hydroControlParameter = 0.0f;
	DynArray<Point> forces = {};
	DynArray<Point> momentums = {};
	bool onGround = false, onRoof = false, onLeftWall = false, onRightWall = false, onJump = false, onDoubleJump = false, onWallJump = false, onDash = false, onAir = false, onWater = false;
	int dashCount = 0;
	//Will be deleted
	bool liftAeroActive = false;
	bool buoyancyActive = false;
};

// ------------------------------------------------------------------------------------------------------------------------------

class Physics
{
public:

	Physics(Render* render, GuiManager* gui);

	virtual ~Physics();

	bool Start();

	//Update and step your physics
	bool Update(float dt);

	//Draw your physic bodies. First argument shows specific propierties from the given DynamicBody*
	void Draw(DynamicBody* dB = nullptr);

	bool CleanUp();

public:
	//Creates a new body with a rectangular form
	Body* CreateBody(BodyType bodyType, Point position = { 0.0f,0.0f }, Rect rect = {}, Point velocity = { 0.0f,0.0f }, Point gravity = { 0.0f,0.0f }, float mass = 1);
	//Creates a new body with a circular form
	Body* CreateBody(BodyType bodyType, Point position = { 0.0f,0.0f }, CircleCollider collider = {}, Point velocity = { 0.0f,0.0f }, Point gravity = { 0.0f,0.0f }, float mass = 1);
	//Sets global gravity for all bodies. {0.0f, 0.0f} for no gravity
	void SetGlobalGravity(Point gravity);
	//Sets global restitution coeficient for all bodies. {0.0f, 0.0f} for perfect elastic restitution
	void SetGlobalRestitution(Point restCoeficient);
	//Sets global friction for all bodies. {0.0f, 0.0f} for no friction
	void SetGlobalFriction(Point frictionCoeficient);
	//Sets a default physics presset
	void SetPhysicsPreset(PhysicsPreset phPreset);
	//Sets a default scenario presset
	void SetScenarioPreset(ScenarioPreset sPreset);
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
	//Returns the body which collisions are calculated with player
	Body* ReturnPlayerCollision() 
	{
		return collidingPlayer;
	}
	//Sets body as the main player, for specific functions of it. Returns false if there is already one body setted as player.
	bool SetBodyAsPlayer(Body* b);

public:
	bool debug = false;
	bool debugBools = false;

private:
	// Debug
	void Step(float dt);
	void Integrate(DynamicBody* item, float dt);
	Body* CheckCollisions(Body* body, Point prevPos);
	Direction DirectionDetection(Point currPos, Point prevPos);
	Direction InvertDirection(Direction dir);
	void ChangeGravityAcceleration(Point acceleration);
	void ChangeRestitutionCoeficient(Point restCoeficient);
	void ChangeFrictionCoeficient(Point frictCoeficient);
	void DebugDraw();
	void DebugBools(DynamicBody* dB = nullptr, float dt = 0.0f);
	void ResetBodyBools();

private:
	std::vector<Body*> bodies;
	Collision utils;
	Point globalGravity = {};
	Point globalRestitution = {};
	Point globalFriction = {};
	bool globalGravityActive = false;
	bool globalRestitutionActive = false;
	bool globalFrictionActive = false;
	bool globalPause = false;
	Body* collidingPlayer = nullptr;

	Render* render = nullptr;
	GuiManager* gui = nullptr;
};

#endif