#pragma once

#include "Body.h"
#include "DynArray.h"

class DynamicBody : public Body
{
public:

	DynamicBody(Rect rect = { 0.0f, 0.0f, 1.0f, 1.0f }, Point velocity = { 0, 0 }, Point gravity = {0, 0}, float mass = 1.0f);

	// Set an specific gravity for this body. Not compatible with SetGlobalGravity() being active. Desable SetGlobalgravity() by inputting SetGlobalGravity(0.0f, 0.0f); or deleting the function
	void SetGravityAcceleration(Point& gravity);
	// Set an constant velocity for this body.
	void SetConstantVelocity(Point& constVelocity);
	//// Allow body to move given an specific force. It needs an input conditional ("if (LEFT ARROW REPEATED) Move(...)"). It allows to limit the velocity. If {0.0f, 0.0f} is inputted, velocity limit will not occur
	//void Move(float newtons = 5.0f, Direction dir = Direction::LEFT, float velocityLimiter = 0.0f);
	//// Allow body to dash given an specific force. It needs an input conditional ("if (LEFT ARROW DOWN) Dash(...)"). It only works on Air. "dashLimit" argument sets how many times can a body dash on air. If limit = 0, dash limit will not occur
	//void Dash(float newtons = 400.0f, DashDirection dDir = DashDirection::LEFT, uint dashLimit = 0);
	// Allow body to jump given an specific force. It needs an input conditional ("if (SPACE BAR PRESSED) Jump(...)"). Input true/false for double jump ability
	void Jump(float newtonsY = 300.0f, bool doubleJump = false);
	// Allow body to jump from a wall given an specific force. It needs an input conditional ("if (SPACE BAR PRESSED) WallJump(...)"). Input true/false for wall friction descendence
	void WallJump(Point newtons = {200.0f, 200.0f}, bool wallFriction = false);
	// Applies an specific force to this body
	inline void ApplyForce(Point newtons) { forces.PushBack(newtons); }
	// Applies an specific force to this body
	inline void ApplyForce(float newtonsX, float newtonsY) { forces.PushBack({ newtonsX, newtonsY }); }
	// Applies an specific force to this body
	void ApplyMomentum(Point momentum);
	// Applies an specific force to this body
	void ApplyMomentum(float momentumX, float momentumY);
	////This function returns a debug bool by inputting which one you want to receive
	//bool GetDebugBools(DebugBools dBool) const
	//{
	//	bool boolSelec = false;
	//	switch (dBool)
	//	{
	//	case DebugBools::GROUND: boolSelec = onGround; break;
	//	case DebugBools::ROOF: boolSelec = onRoof; break;
	//	case DebugBools::LEFT_WALL: boolSelec = onLeftWall; break;
	//	case DebugBools::RIGHT_WALL: boolSelec = onRightWall; break;
	//	case DebugBools::JUMP: boolSelec = onJump; break;
	//	case DebugBools::DOUBLE_JUMP: boolSelec = onDoubleJump; break;
	//	case DebugBools::WALL_JUMP: boolSelec = onWallJump; break;
	//	case DebugBools::DASH: boolSelec = onDash; break;
	//	}
	//	return boolSelec;
	//}

	// Restart the accelerations/velocities applied to the body
	void ResetForces();
	//// Change the position of the body
	//void SetPosition(Point newPosition)
	//{
	//	position = newPosition;
	//	rect.x = newPosition.x;
	//	rect.y = newPosition.y;
	//}

private:

	void SecondNewton();
	void FirstBuxeda();
	void ApplyFriction(float dt);
	void ApplyBuoyancy();
	void ApplyHidroDrag();
	void ResetBools();

private:
	friend class Physics;
	friend class Body;

	Point velocity;
	Point gravityAcceleration;
	Point acceleration = {};
	Point sumForces = { 0.0f, 0.0f };
	Point sumMomentum = {0.0f, 0.0f};
	Point coeficientRestitution = { 1.0f,1.0f };
	Point frictionCoeficient = { 0.0f,0.0f };
	float hydroControlParameter = 0.0f;
	DynArray<Point> forces = {}; // TODO: Canviar a std (forces, momentum)
	DynArray<Point> momentums = {};
	bool onGround = false, onRoof = false, onLeftWall = false, onRightWall = false, onJump = false, onDoubleJump = false, onWallJump = false, onDash = false, onAir = false, onWater = false;
	int dashCount = 0;
	//Will be deleted
	bool liftAeroActive = false;
	bool buoyancyActive = false;
};
