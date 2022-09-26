#include "NoPhysicsLibrary.h"
#include <stdint.h>
#include "Define.h"
#include "MathUtils.h"
#include "External/miniaudio/miniaudio.h"

NPL::NPL()
{
}

NPL::~NPL()
{
	CleanUp();
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
	// AUDIO
	audio->CleanUp();

	// BODIES
	if (!bodies.empty())
	{
		for (Body* b : bodies)
		{
			RELEASE(b);
		}
	}
	bodies.clear();
	bodies.shrink_to_fit();

	// LISTENER
	listener = nullptr;

	// PHYSICS
	physics->CleanUp();

	// SOUND DATA
	if (!soundDataList.empty())
	{
		for (Body* b : bodies) RELEASE(b);
	}
	soundDataList.clear();
	soundDataList.shrink_to_fit();

	// Gas Locations
	if (!gasLocations.empty())
	{
		for (unsigned int* index : gasLocations) RELEASE(index);
	}
	gasLocations.clear();
	gasLocations.shrink_to_fit();

}

BodyCreation NPL::CreateBody(Rect rectangle, float mass)
{
	//Library not initialized. Call NPL::Init() first
	assert(physics != nullptr);

	return BodyCreation(rectangle, mass, &bodies, &gasLocations, physics);
}

LibraryConfig NPL::Configure()
{
	return LibraryConfig(&panRange);
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
	bool ret = false;
	for (Body* b : bodies)
	{
		if (b->GetClass() != BodyClass::DYNAMIC_BODY) continue;

		if (!MathUtils::CheckCollision(b->GetRect(), limits))
		{
			ret = true;
			DestroyBody(b);
		}
	}

	return ret;
}

bool NPL::DeathLimit(Rect limits, DynamicBody* body)
{
	if (!body || body->GetClass() != BodyClass::DYNAMIC_BODY) return false;

	bool ret = false;
	
	if (!MathUtils::CheckCollision(body->GetRect(), limits))
	{
		ret = true;
		DestroyBody(body);
	}

	return ret;
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
	{
		float downLimitY = wSize.y * 0.93f;
		float rightLimitX = wSize.x * 0.96f;
		//Limits
		CreateBody({ 0,          0,               wSize.x, wSize.y - downLimitY }, 1).Static();
		CreateBody({ 0,          0, wSize.x - rightLimitX,              wSize.y }, 1).Static();
		CreateBody({ 0, downLimitY,               wSize.x, wSize.y - downLimitY }, 1).Static();
		CreateBody({ rightLimitX,          0, wSize.x - rightLimitX,              wSize.x }, 1).Static();
		break;
	}
	case ScenarioPreset::CORRIDOR_SCENARIO_PRESET:
	{
		float downLimitY = wSize.y * 0.93f;
		float rightLimitX = wSize.x * 0.96f;
		//Limits
		CreateBody({                  0,          0,                  4280, wSize.y - downLimitY }, 1).Static();
		CreateBody({                  0,          0, wSize.x - rightLimitX,              wSize.y }, 1).Static();
		CreateBody({                  0, downLimitY,                  4280, wSize.y - downLimitY }, 1).Static();
		CreateBody({ 3000 + rightLimitX,          0, wSize.x - rightLimitX,              wSize.x }, 1).Static();
		break;
	}

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
	// If no listener & environment is void
	if (!listener && IsVoid())
	{
		for (Body* b : bodies)
		{
			for (AcousticData* data : b->acousticDataList)
			{
				RELEASE(data);
			}
			b->acousticDataList.clear();
		}
		return;
	}

	for (Body* b : bodies)
	{
		if (b->acousticDataList.empty()) continue;

		if (!listener)
		{
			NoListenerLogic(b);
			continue;
		}

		GasBody* environment = GetEnvironmentBody(b->GetRect());

		if (!environment)
		{
			for (AcousticData* data : b->acousticDataList) RELEASE(data);
			b->acousticDataList.clear();
			continue;
		}
		
		ListenerLogic(b, environment);
	}
}

void NPL::NoListenerLogic(Body* b)
{
	for (AcousticData* data : b->acousticDataList)
	{
		float volume = data->spl / maxSPL;
		soundDataList.push_back(new SoundData(data->index, 0, volume, 0));
		RELEASE(data);
	}
	b->acousticDataList.clear();
}

void NPL::ListenerLogic(Body* b, GasBody* environment)
{
	// If listener emit sound
	if (b->GetId() == listener->id)
	{
		for (AcousticData* data : b->acousticDataList)
		{
			soundDataList.emplace_back(new SoundData(data->index));
			RELEASE(data);
		}
		b->acousticDataList.clear();
		return;
	}

	//-Todone: group operations of similar thematics (pan with pan, time with time...)
	for (AcousticData* data : b->acousticDataList)
	{
		// Get the distance between Body & Listener
		float distance = listener->GetPosition().Distance(data->position);

		float pan = ComputePanning(distance, data->position.x);

		float volume = ComputeVolume(distance, data->spl);

		float timeDelay = ComputeTimeDelay(distance, environment) / 14;

		soundDataList.emplace_back(new SoundData(data->index, pan, volume, timeDelay));
		RELEASE(data);
	}
	b->acousticDataList.clear();
}

GasBody* NPL::GetEnvironmentBody(Rect body)
{
	for (unsigned int* index : gasLocations)
	{
		if (MathUtils::CheckCollision(body, bodies[*index]->GetRect()))
			return (GasBody*)bodies[*index];
	}

	return nullptr;
}

float NPL::ComputePanning(float distance, float bodyX)
{
	// Check direction for audio panning (50L(neg) or 50R(pos))
	float direction = 1;
	if (listener->GetPosition().x < bodyX) direction *= -1;

	// Narrow down distance over Range for panning operations
	if (distance > panRange) distance = panRange;
	if (distance < -panRange) distance = -panRange;

	// Compute pan (normalized between [ 1, -1])
	return distance / -panRange;
}

float NPL::ComputeVolume(float distance, float spl)
{
	// Compute the sound attenuation over distance
	// Final SPL = Initial SPL - 20*Log(distance / 1)
	float fSPL = spl - 20 * MathUtils::Log(distance);

	// Transform volume from db to linear [ 0, 1])
	return MathUtils::LogToLinear(fSPL, maxSPL) / maxVolume;
}

float NPL::ComputeTimeDelay(float distance, GasBody* environment)
{
	//-Todone: Fer de moment un checkcollision d'un gasbody i fer llògica
	// Calculate delay time
	// 1. Vel = sqrt( lambda * Pa / ro )
	// 2. Time = distance / vel
	float timeDelay = 0;
	float vel = MathUtils::Sqrt(environment->GetHeatRatio() * environment->GetPressure() / environment->GetDensity());
	return distance / vel;
}

void NPL::StepAudio()
{
	audio->Update();

	if (soundDataList.empty()) return;

	for (SoundData* data : soundDataList)
	{
		audio->Playback(data);
		RELEASE(data);
	}

	soundDataList.clear();
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
