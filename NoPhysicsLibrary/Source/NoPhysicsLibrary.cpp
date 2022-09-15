#include "NoPhysicsLibrary.h"
#include <stdint.h>

// Deletes a buffer
#define RELEASE( x ) \
	{						\
	if( x != NULL )		\
		{					  \
	  delete x;			\
	  x = NULL;			  \
		}					  \
	}

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

void NPL::Update(float dt)
{
	if (GetGlobalPause()) return;

	StepPhysics(dt);
	StepAcoustics();
	StepAudio();

}

void NPL::CleanUp()
{
	physics->CleanUp();
	audio->CleanUp();
}

BodyCreation NPL::CreateBody(Rect rectangle, float mass)
{
	//Library not initialized. Call NPL::Init() first
	assert(physics != nullptr);

	return BodyCreation(rectangle, mass, &bodies, physics, audio);
}

void NPL::DestroyScenario()
{
	for (Body* b : bodies)
	{
		if (b->GetClass() == BodyClass::STATIC_BODY) DestroyBody(b);
	}
}

Rect NPL::ReturnScenarioRect()
{
	Rect first = bodies.front()->GetRect();
	Point minP = { first.x, first.y };
	Point maxP = { first.x + first.w, first.y + first.h};

	for (Body* body : bodies)
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

void NPL::SetGlobalGravity(Point vector)
{
	physics->globalGravity = vector;
}

inline Point NPL::GetGlobalFriction() const
{
	return physics->globalFriction;
}

void NPL::SetGlobalFriction(Point vector)
{
	physics->globalFriction = vector;
}

inline Point NPL::GetGlobalRestitution() const
{
	return physics->globalRestitution;
}

void NPL::SetGlobalRestitution(Point vector)
{
	physics->globalRestitution = vector;
}

bool NPL::DeathLimit(Rect limits)
{
	//-TOCHECK: It is better to update all bodies in case we have to delete 2, or just delete 1 per frame?
	for (Body* b : bodies)
	{
		if (b->GetClass() == BodyClass::DYNAMIC_BODY && !physics->CheckCollision(b->GetRect(), limits))
		{
			DestroyBody(b);
			return true; //First body out deleted, if other one is out, deleted next frame
		}
	}

	return false;
}

bool NPL::DestroyBody(Body* body)
{
	if (EraseBody(body))
	{
		RELEASE(body);
		return true;
	}

	return false;
}

void NPL::SetListener(Body* listener)
{
	this->listener = listener;
}

void NPL::LoadSound(const char* path)
{
	audio->LoadSound(path);
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

//- Private --------------------------------------------------------------------------------

void NPL::StepPhysics(float dt)
{
	//-TOCHECK: Think about a pointer (std::vector<Body*>*) inside physics class & making for inside Step function
	for (Body* b : bodies) physics->Step(b, dt);

	physics->Declip(&bodies);

}

void NPL::StepAcoustics()
{
	if (!listener) return;

	//-TODO: Cada body tenir una llista SoundData, updatejar tots els bodies i emplenar soundList.

	//-TODO: Updatejar llogica de acustica aqu�. SoundData list a NPL
	for (SoundData* data : soundList)
	{
		float distance = listener->GetPosition().Distance(data->position);
		if (distance > panRadius) distance = panRadius;
		if (distance < -panRadius) distance = -panRadius;

		float pan = (distance * 1) / -panRadius;
		float volume = (distance * 1) / panRadius;
		if (volume < 0) volume *= -1;
		volume = 1 - volume;

		data->Set(pan, volume);
	}
}

void NPL::StepAudio()
{
	if (soundList.empty()) return;

	for (SoundData* data : soundList) audio->Update(data);

}

bool NPL::EraseBody(Body* body)
{
	std::vector<Body*>::const_iterator it;
	for (it = bodies.begin(); it != bodies.end(); ++it)
	{
		if (body->GetId() == (*it)->GetId())
		{
			bodies.erase(it);
			bodies.shrink_to_fit();
			return true;
		}
	}

	return false;
}
