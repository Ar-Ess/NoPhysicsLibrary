#include "NoPhysicsLibrary.h"
#include "Physics.h"
#include <stdint.h>

NPL::NPL()
{
}

void NPL::Init()
{
	// You've alreay initialized the library once
	assert(physics == nullptr && audio == nullptr);

	physics = new Physics();
	audio = new Audio();
}

void NPL::CleanUp()
{
	physics->CleanUp();
	// MiniAudio CleanUp
}

BodyCreation NPL::CreateBody(Rect rectangle, float mass)
{
	//Library not initialized. Call NPL::Init() first
	assert(physics != nullptr);

	return BodyCreation(rectangle, mass, &physics->bodies);
}

void NPL::Step(float dt)
{
	if (GetGlobalPause()) return;

	physics->Update(dt);

}

void NPL::DestroyScenario()
{
	for (Body* b : physics->bodies)
	{
		if (b->GetClass() == BodyClass::STATIC_BODY) physics->DestroyBody(b);
	}
}

Rect NPL::ReturnScenarioRect()
{
	Rect first = physics->bodies.front()->GetRect();
	Point minP = { first.x, first.y };
	Point maxP = { first.x + first.w, first.y + first.h};

	for (Body* body : physics->bodies)
	{
		Rect bodyRect = body->GetRect();
		if (bodyRect.x + bodyRect.w > maxP.x) maxP.x = bodyRect.x + bodyRect.w;
		if (bodyRect.y + bodyRect.h > maxP.y) maxP.y = bodyRect.y + bodyRect.h;
		if (bodyRect.x < minP.x) minP.x = bodyRect.x;
		if (bodyRect.y < minP.y) minP.y = bodyRect.y;
	}

	return Rect{ minP.x, minP.y, maxP.x - minP.x, maxP.y - minP.y};
}

inline bool NPL::GetGlobalPause() const
{
	return physics->globals.Get(0);

}

inline Point NPL::GetGlobalGravity() const
{
	return physics->globalGravity;
}

void NPL::SetGlobalGravity(Point magnitude)
{
	physics->globalGravity = magnitude;
}

inline Point NPL::GetGlobalFriction() const
{
	return physics->globalFriction;
}

inline void NPL::SetGlobalFriction(Point magnitude)
{
	physics->globalFriction = magnitude;
}

inline Point NPL::GetGlobalRestitution() const
{
	return physics->globalRestitution;
}

inline void NPL::SetGlobalRestitution(Point magnitude)
{
	physics->globalRestitution = magnitude;
}

bool NPL::DeathLimit(Rect limits)
{
	for (Body* b : physics->bodies)
	{
		if (b->GetClass() == BodyClass::DYNAMIC_BODY && !physics->CheckCollision(b->GetRect(), limits))
		{
			DestroyBody(b);
			return true;
		}
	}

	return false;
}

bool NPL::DestroyBody(Body* body)
{
	return physics->DestroyBody(body);
}

void NPL::PausePhysics(bool pause)
{
	physics->globals.Set(0, pause);
}

void NPL::SetScenarioPreset(ScenarioPreset sPreset, Point wSize)
{
	DestroyScenario();

	switch (sPreset)
	{
	case ScenarioPreset::LIMITS_SCENARIO_PRESET:
		float downLimitY = wSize.y * 0.93f;
		float rightLimitX = wSize.x * 0.96f;
		//Limits
		CreateBody({           0,          0,               wSize.x, wSize.y - downLimitY }, 1);
		CreateBody({           0,          0, wSize.x - rightLimitX,              wSize.y }, 1);
		CreateBody({           0, downLimitY,               wSize.x, wSize.y - downLimitY }, 1);
		CreateBody({ rightLimitX,          0, wSize.x - rightLimitX,              wSize.x }, 1);
		break;

	//case ScenarioPreset::PLATFORMER_SCENARIO_PRESET:
	//	//Limits
	//	CreateBody({ 0,   0, 1280,  50 }, 1);
	//	CreateBody({ 0,   0,   50, 720 }, 1);
	//	CreateBody({ 0, 670, 1280,  50 }, 1);
	//	CreateBody({ 1230, 0,  50, 720 }, 1);
	//	//Obstacles
	//	CreateBody(BodyType::STATIC_BODY, Point{ 340, 490 }, { 340, 490, 600, 25 }, { 0, 0 }, { 0, 0 }, 1);
	//	CreateBody(BodyType::STATIC_BODY, Point{ 540, 220 }, { 540, 220, 200, 25 }, { 0, 0 }, { 0, 0 }, 1);
	//	break;

	//case ScenarioPreset::WALLJUMP_SCENARIO_PRESET:
	//	//Limits
	//	CreateBody(BodyType::STATIC_BODY, Point{ 0, 0 }, { 0, 0, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
	//	CreateBody(BodyType::STATIC_BODY, Point{ 0, 0 }, { 0, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
	//	CreateBody(BodyType::STATIC_BODY, Point{ 0, 670 }, { 0, 670, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
	//	CreateBody(BodyType::STATIC_BODY, Point{ 1230, 0 }, { 1230, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
	//	//Obstacles
	//	CreateBody(BodyType::STATIC_BODY, Point{ 200, 200 }, { 200, 200, 25, 400 }, { 0, 0 }, { 0, 0 }, 1);
	//	CreateBody(BodyType::STATIC_BODY, Point{ 500, 200 }, { 500, 200, 25, 400 }, { 0, 0 }, { 0, 0 }, 1);
	//	CreateBody(BodyType::STATIC_BODY, Point{ 780, 200 }, { 780, 200, 25, 400 }, { 0, 0 }, { 0, 0 }, 1);
	//	CreateBody(BodyType::STATIC_BODY, Point{ 1080, 200 }, { 1080, 200, 25, 400 }, { 0, 0 }, { 0, 0 }, 1);
	//	break;
	}
}
