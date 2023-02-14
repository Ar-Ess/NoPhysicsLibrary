#include "NoPhysicsLibrary.h"
#include <stdint.h>
#include "Define.h"
#include "PhysMath.h"
#include "External/miniaudio/miniaudiodev.h"

NPL::NPL()
{
}

NPL::~NPL()
{
}

void NPL::Init(float pixelsPerMeter)
{
	// You've alreay initialized the library once
	assert(physics == nullptr && audio == nullptr, "ERROR: NPL variable initialized twice. Call CleanUp() first before initializing the variable again.");

	physics = new Physics(&bodies, &physicsConfig, &gasIndex, &liquidIndex, &pixelsToMeters, &physIterations);
	audio = new Audio();

	// Pixels To Meters = [ m / pxl ]
	pixelsToMeters = pixelsPerMeter > 0 ? 1 / pixelsPerMeter : 1;

	bodyCreation = new BodyCreation(
		&bodies, 
		&gasIndex, 
		&liquidIndex, 
		physics, 
		&pixelsToMeters);

	libraryConfig = new LibraryConfig(
		&panRange,
		&physicsConfig,
		&physics->globalGravity,
		&physics->globalRestitution,
		&physics->globalFriction,
		&listener,
		&pixelsToMeters,
		&ptmRatio,
		&physIterations,
		&notifier
	);

	getData = new GetData(
		&bodies, 
		&physics->collisions, 
		&physicsConfig,
		&physics->globalGravity, 
		&physics->globalFriction, 
		&physics->globalRestitution, 
		&physics->globals,
		&pixelsToMeters
	);
}

void NPL::Update(float* dt)
{
	assert(physics != nullptr && audio != nullptr, "ERROR: NPL variable not initialized. Call Init() first.");

	if (IsGlobalPause()) return;

	//INFO: Uniform forces independent from space use InUnit::IN_PIXELS, otherwise InUnit::IN_METERS
	UpdateNotifier();

	StepPhysics(*dt);
	StepAcoustics();
	StepAudio(dt);

	UpdateStates();
}

void NPL::CleanUp()
{
	assert(physics != nullptr && audio != nullptr, "ERROR: NPL variable not initialized. Call Init() first.");

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
	if (!gasIndex.empty())
	{
		for (unsigned int* index : gasIndex) RELEASE(index);
	}
	gasIndex.clear();
	gasIndex.shrink_to_fit();

}

const BodyCreation* NPL::CreateBody(PhysRect rectangle)
{
	//Library not initialized. Call NPL::Init() first
	assert(physics != nullptr, "ERROR: NPL variable not initialized. Call Init() first.");

	rectangle *= pixelsToMeters;

	return bodyCreation->Access(rectangle);
}

const BodyCreation* NPL::CreateBody(float x, float y, float width, float height)
{
	//Library not initialized. Call NPL::Init() first
	assert(physics != nullptr, "ERROR: NPL variable not initialized. Call Init() first.");

	PhysRect rectangle = PhysRect( x, y, width, height ) * pixelsToMeters;

	return bodyCreation->Access(rectangle);
}

const LibraryConfig* NPL::Configure()
{
	assert(physics != nullptr && audio != nullptr, "ERROR: NPL variable not initialized. Call Init() first.");
	return libraryConfig->Access();
}

const GetData* NPL::Get()
{
	assert(physics != nullptr && audio != nullptr, "ERROR: NPL variable not initialized. Call Init() first.");
	return getData->Access();
}

bool NPL::DestroyBody(Body* body)
{
	assert(physics != nullptr && audio != nullptr, "ERROR: NPL variable not initialized. Call Init() first.");
	if (EraseBody(body->GetId()))
	{
		RELEASE(body);
		return true;
	}

	return false;
}

bool NPL::DestroyBody(int id)
{
	assert(physics != nullptr && audio != nullptr, "ERROR: NPL variable not initialized. Call Init() first.");

	Body* body = nullptr;
	if (EraseBody(id, body))
	{
		RELEASE(body);
		return true;
	}

	return false;
}

bool NPL::DestroyBody(BodyClass clas)
{
	assert(physics != nullptr && audio != nullptr, "ERROR: NPL variable not initialized. Call Init() first.");

	bool ret = true;
	for (Body* b : bodies)
	{
		if (b->GetClass() == clas)
		{
			bool destroyed = DestroyBody(b);
			if (!destroyed) ret = false;
		}
	}

	return ret;
}

void NPL::LoadSound(const char* path)
{
	assert(audio != nullptr, "ERROR: NPL variable not initialized. Call Init() first.");

	audio->LoadSound(path);
}

void NPL::PausePhysics(bool pause)
{
	physics->globals.Set(0, pause);
}

const PhysRect NPL::ReturnScenarioRect() const
{
	if (bodies.empty()) return PhysRect();

	PhysRect first = bodies.front()->Rect(InUnit::IN_PIXELS);
	PhysVec minP = { first.x, first.y };
	PhysVec maxP = { first.x + first.w, first.y + first.h };

	for (Body* body : bodies)
	{
		PhysRect bodyRect = body->Rect(InUnit::IN_PIXELS);
		if (bodyRect.x + bodyRect.w > maxP.x) maxP.x = bodyRect.x + bodyRect.w;
		if (bodyRect.y + bodyRect.h > maxP.y) maxP.y = bodyRect.y + bodyRect.h;
		if (bodyRect.x < minP.x) minP.x = bodyRect.x;
		if (bodyRect.y < minP.y) minP.y = bodyRect.y;
	}

	return PhysRect{ minP.x, minP.y, maxP.x - minP.x, maxP.y - minP.y };
}

void NPL::SetScenarioPreset(ScenarioPreset preset, PhysVec windowSize, std::vector<StaticBody*>* scenarioBodies)
{
	assert(physics != nullptr && audio != nullptr, "ERROR: NPL variable not initialized. Call Init() first.");

	switch (preset)
	{
	case ScenarioPreset::LIMITS_SCENARIO_PRESET:
	{
		float downLimitY = windowSize.y * 0.93f;
		float rightLimitX = windowSize.x * 0.96f;

		const unsigned int maxStatic = 4;
		StaticBody* scenario[maxStatic] = { nullptr, nullptr, nullptr, nullptr };
		//Limits
		scenario[0] = CreateBody({ 0.f,          0.f,               windowSize.x, windowSize.y - downLimitY })->Static();
		scenario[1] = CreateBody({ 0.f,          0.f, windowSize.x - rightLimitX,              windowSize.y })->Static();
		scenario[2] = CreateBody({ 0.f, downLimitY,               windowSize.x, windowSize.y - downLimitY })->Static();
		scenario[3] = CreateBody({ rightLimitX,          0.f, windowSize.x - rightLimitX,              windowSize.y })->Static();

		if (scenarioBodies != nullptr) for (unsigned int i = 0; i < maxStatic; ++i) scenarioBodies->emplace_back(scenario[i]);

		break;
	}

	case ScenarioPreset::CORRIDOR_SCENARIO_PRESET:
	{
		float downLimitY = windowSize.y * 0.93f;
		float rightLimitX = windowSize.x * 0.96f;

		const unsigned int maxStatic = 4;
		StaticBody* scenario[maxStatic] = { nullptr, nullptr, nullptr, nullptr };

		//Limits
		CreateBody({ 0.f,          0.f,                       4280.f, windowSize.y - downLimitY })->Static();
		CreateBody({ 0.f,          0.f, windowSize.x - rightLimitX,              windowSize.y })->Static();
		CreateBody({ 0.f, downLimitY,                       4280.f, windowSize.y - downLimitY })->Static();
		CreateBody({ 3000 + rightLimitX,          0.f, windowSize.x - rightLimitX,              windowSize.x })->Static();

		if (scenarioBodies != nullptr) for (unsigned int i = 0; i < maxStatic; ++i) scenarioBodies->emplace_back(scenario[i]);

		break;
	}

	case ScenarioPreset::PLATFORMER_SCENARIO_PRESET:
	{
		float downLimitY = windowSize.y * 0.93f;
		float rightLimitX = windowSize.x * 0.96f;

		const unsigned int maxStatic = 6;
		StaticBody* scenario[maxStatic] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

		//Limits
		scenario[0] = CreateBody({ 0.f,          0.f,               windowSize.x, windowSize.y - downLimitY })->Static();
		scenario[1] = CreateBody({ 0.f,          0.f, windowSize.x - rightLimitX,              windowSize.y })->Static();
		scenario[2] = CreateBody({ 0.f, downLimitY,               windowSize.x, windowSize.y - downLimitY })->Static();
		scenario[3] = CreateBody({ rightLimitX,          0.f, windowSize.x - rightLimitX,              windowSize.y })->Static();

		//Obstacles
		scenario[4] = CreateBody({ windowSize.x * 0.26f, windowSize.y * 0.62f, windowSize.x * 0.46f, windowSize.y * 0.034f })->Static();
		scenario[5] = CreateBody({ windowSize.x * 0.42f, windowSize.y * 0.30f, windowSize.x * 0.15f, windowSize.y * 0.034f })->Static();

		if (scenarioBodies != nullptr) for (unsigned int i = 0; i < maxStatic; ++i) scenarioBodies->emplace_back(scenario[i]);

		break;
	}

	case ScenarioPreset::WALLJUMP_SCENARIO_PRESET:
	{
		float downLimitY = windowSize.y * 0.93f;
		float rightLimitX = windowSize.x * 0.96f;

		const unsigned int maxStatic = 8;
		StaticBody* scenario[maxStatic] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

		//Limits
		scenario[0] = CreateBody({ 0.f,          0.f,               windowSize.x, windowSize.y - downLimitY })->Static();
		scenario[1] = CreateBody({ 0.f,          0.f, windowSize.x - rightLimitX,              windowSize.y })->Static();
		scenario[2] = CreateBody({ 0.f, downLimitY,               windowSize.x, windowSize.y - downLimitY })->Static();
		scenario[3] = CreateBody({ rightLimitX,          0.f, windowSize.x - rightLimitX,    windowSize.y })->Static();

		//Obstacles
		CreateBody({ windowSize.x * 0.23f, windowSize.y * 0.27f, windowSize.x * 0.02f, windowSize.y * 0.55f })->Static();
		CreateBody({ windowSize.x * 0.40f, windowSize.y * 0.27f, windowSize.x * 0.02f, windowSize.y * 0.55f })->Static();
		CreateBody({ windowSize.x * 0.61f, windowSize.y * 0.27f, windowSize.x * 0.02f, windowSize.y * 0.55f })->Static();
		CreateBody({ windowSize.x * 0.84f, windowSize.y * 0.27f, windowSize.x * 0.02f, windowSize.y * 0.55f })->Static();
		break;
	}
	}
}

void NPL::SetPhysicsPreset(PhysicsPreset preset)
{
	assert(physics != nullptr, "ERROR: NPL variable not initialized. Call Init() first.");

	PhysVec friction = {};
	PhysVec gravity = {};
	PhysVec restitution = {};

	switch (preset)
	{
	case PhysicsPreset::DEFAULT_PHYSICS_PRESET:
		friction = { 0.5f, 0.0f };
		gravity = { 0.0f, 9.8f };
		restitution = { 0.0f, 0.0f };
		break;

	case PhysicsPreset::NO_PHYSIC_PRESET:
		break;

	default:
		break;
	}

	physics->globalFriction = friction;
	physics->globalGravity = gravity;
	physics->globalRestitution = restitution;
}

//- Private --------------------------------------------------------------------------------

void NPL::StepPhysics(float dt)
{
	for (Body* body : bodies) physics->Step(body, dt);

	physics->SolveCollisions(&bodies);
}

void NPL::UpdateStates()
{
	for (Body* b : bodies)
	{
		if (b->clas != BodyClass::DYNAMIC_BODY) continue;
		DynamicBody* dB = (DynamicBody*)b;

		// Detect moving
		if (PhysMath::Abs(PhysMath::Abs(dB->velocity.x)) > 0.001f || PhysMath::Abs(PhysMath::Abs(dB->velocity.y)) > 0.001f)
		{
			if (!dB->prevBodyState.Get((int)BodyState::MOVING)) dB->bodyStateEnter.Set((int)BodyState::MOVING, true);
			dB->bodyStateStay.Set((int)BodyState::MOVING, true);
		}

		// Detect floating (no collision with solids)
		bool floating = true;
		for (unsigned int i = 1; i < 5; ++i)
		{
			if (dB->IsBodyStill((BodyState)i))
			{
				floating = false;
				break;
			}
		}
		if (floating)
		{
			dB->bodyStateStay.Set((int)BodyState::FLOATING, true);
			if (!dB->prevBodyState.Get((int)BodyState::FLOATING)) dB->bodyStateEnter.Set((int)BodyState::FLOATING, true);
		}

		// Detect liquid & Gas
		bool fullLiquidState = false;
		float totalArea = 0.0f;
		for (unsigned int* i : liquidIndex)
		{
			if (PhysMath::CheckCollision(b->Rect(InUnit::IN_METERS), bodies[*i]->Rect(InUnit::IN_METERS)))
			{
				if (!dB->prevBodyState.Get((int)BodyState::IN_LIQUID)) dB->bodyStateEnter.Set((int)BodyState::IN_LIQUID, true);
				dB->bodyStateStay.Set((int)BodyState::IN_LIQUID, true);
				totalArea += PhysMath::IntersectRectangle(b->Rect(InUnit::IN_METERS), bodies[*i]->Rect(InUnit::IN_METERS)).Area();
				fullLiquidState = (0.0001f > PhysMath::Abs(b->Rect(InUnit::IN_METERS).Area() - totalArea));
				if (fullLiquidState) break;
			}
		}

		if (!fullLiquidState)
		{
			for (unsigned int* i : gasIndex)
			{
				if (PhysMath::CheckCollision(b->Rect(InUnit::IN_METERS), bodies[*i]->Rect(InUnit::IN_METERS)))
				{
					if (!dB->prevBodyState.Get((int)BodyState::IN_GAS)) dB->bodyStateEnter.Set((int)BodyState::IN_GAS, true);
					dB->bodyStateStay.Set((int)BodyState::IN_GAS, true);
					break;
				}
			}
		}

		dB->SetPreviousBodyState();
	}
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
		if (b->acousticDataList.empty() || !b->properties.Get(2)) continue;

		if (!listener)
		{
			NoListenerLogic(b);
			continue;
		}

		GasBody* environment = GetEnvironmentBody(b->Rect(InUnit::IN_METERS));

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
		if (data->index < 0 || data->index >= audio->GetSoundSize())
		{
			RELEASE(data);
			continue;
		}
		float volume = data->spl / maxSPL;
		soundDataList.emplace_back(new SoundData(data->index, 0, volume, 0));
		RELEASE(data);
	}
	b->acousticDataList.clear();
}

void NPL::ListenerLogic(Body* b, GasBody* environment)
{
	// If listener emit sound
	if (b->GetId() == listener->id)
	{
		NoListenerLogic(b);
		return;
	}

	for (AcousticData* data : b->acousticDataList)
	{
		if (data->index < 0 || data->index >= audio->GetSoundSize())
		{
			RELEASE(data);
			continue;
		}

		// Get the distance between Body & Listener
		float distance = PhysMath::Distance((listener->Position(InUnit::IN_METERS) + listener->emissionPoint), data->emissionPosition);

		float pan = ComputePanning(distance, data->emissionPosition.x);

		float volume = ComputeVolume(distance, data->spl);

		float timeDelay = ComputeTimeDelay(distance, environment);

		soundDataList.emplace_back(new SoundData(data->index, pan, volume, timeDelay));
		RELEASE(data);
	}
	b->acousticDataList.clear();
}

GasBody* NPL::GetEnvironmentBody(PhysRect body)
{
	for (unsigned int* index : gasIndex)
	{
		if (PhysMath::CheckCollision(body, bodies[*index]->Rect(InUnit::IN_METERS)))
			return (GasBody*)bodies[*index];
	}

	return nullptr;
}

void NPL::UpdateNotifier()
{
	if (!notifier.IsAnyTrue()) return;

	if (notifier.Get(0)) UpdatePixelsToMeters();

	notifier.Clear();
}

void NPL::UpdatePixelsToMeters()
{
	for (Body* b : bodies)
	{
		switch (b->clas)
		{
		case BodyClass::DYNAMIC_BODY:
		{
			DynamicBody* dB = (DynamicBody*)b;
			dB->GravityOffset(dB->GravityOffset() * ptmRatio);
			break; 
		}
		case BodyClass::LIQUID_BODY:
			break;
		default:
			break;
		}

		b->emissionPoint *= ptmRatio;
		b->rect.x *= ptmRatio;
		b->rect.y *= ptmRatio;
		b->rect.h *= ptmRatio;
		b->rect.w *= ptmRatio;
	}

	panRange *= ptmRatio;
	physics->globalGravity *= ptmRatio;
}

float NPL::ComputePanning(float distance, float bodyX)
{
	// Check direction for audio panning (50L(neg) or 50R(pos))
	float direction = 1;
	if ((listener->Position(InUnit::IN_METERS) + listener->emissionPoint).x < bodyX) direction *= -1;

	// Narrow down distance over Range for panning operations
	if (distance > panRange) distance = panRange;
	if (distance < -panRange) distance = -panRange;

	// Compute pan (normalized between [ 1, -1])
	return distance / panRange * direction;
}

float NPL::ComputeVolume(float distance, float spl)
{
	// Compute the sound attenuation over distance
	// Final SPL = Initial SPL - 20*Log(distance / 1)
	float fSPL = spl - 20 * PhysMath::Log(distance);

	// Transform volume from db to linear [ 0, 1])
	return PhysMath::LogToLinear(fSPL, maxSPL) / maxVolume;
}

float NPL::ComputeTimeDelay(float distance, GasBody* environment)
{
	// Calculate delay time
	// 1. Vel = sqrt( lambda * Pa / ro )
	// 2. Time = distance / vel
	float timeDelay = 0;
	float vel = PhysMath::Sqrt(environment->GetHeatRatio() * environment->GetPressure() / environment->GetDensity(InUnit::IN_METERS));
	return distance / vel;
}

void NPL::StepAudio(float* dt)
{
	audio->Update();

	if (soundDataList.empty()) return;

	for (SoundData* data : soundDataList)
	{
		audio->Playback(data, dt);
		RELEASE(data);
	}

	soundDataList.clear();
}

bool NPL::EraseBody(int id, Body* outBody)
{
	std::vector<Body*>::const_iterator it;
	for (it = bodies.begin(); it != bodies.end(); ++it)
	{
		if (id == (*it)->GetId())
		{
			outBody = (*it);
			bodies.erase(it);
			return true;
		}
	}

	return false;
}
