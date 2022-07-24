#include "NoPhysicsLibrary.h"
#include "Physics.h"

NPL::NPL()
{
}

void NPL::Init()
{
	// You've alreay initialized the library once
	assert(physics == nullptr);

	physics = new Physics();
}

BodyCreation NPL::CreateBody(Rect rectangle, float mass)
{
	//Library not initialized. Call NPL::Init() first
	assert(physics != nullptr);

	return BodyCreation(rectangle, mass, &physics->bodies);
}

bool NPL::Step(float dt)
{
	if (physics->GetGlobalPause()) return true;

	if (physics->GetGlobalGravity()) int i = 0; // ChangeGravityAcceleration(globalGravity);

	if (physics->GetGlobalRestitution()) int i = 0;// ChangeRestitutionCoeficient(globalRestitution);
	//else ChangeRestitutionCoeficient({ 1.0f, 1.0f });

	if (physics->GetGlobalFriction()) int i = 0; // ChangeFrictionCoeficient(globalFriction);

	physics->Update(dt);

	return false;
}

void NPL::DestroyScenario()
{
	std::vector<Body*>::const_iterator it;
	for (it = physics->bodies.begin(); it != physics->bodies.end(); ++it)
	{
		Body* body = (*it);
		if (body->GetBodyClass() == BodyClass::STATIC_BODY) physics->DestroyBody(it);
	}
	return;
}

Rect NPL::ReturnScenarioSize()
{
	// TODO: Funció per retornar el rectangla englobador de tots els cossos
	for (Body* body : physics->bodies)
	{
		Point maxP = { 0, 0 };
		Point minP = { 0, 0 };

		Rect bodyRect = Rect{ body->GetPosition(), body->GetSize() }; // Cambiar a GetRect()
		if (bodyRect.x + bodyRect.w > maxP.x) maxP.x = bodyRect.x;
		if (bodyRect.y + bodyRect.h > maxP.y) maxP.y = bodyRect.y;
	}

	return Rect();
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
