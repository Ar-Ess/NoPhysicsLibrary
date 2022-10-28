#include "NoPhysicsLibrary.h"
#include <stdint.h>
#include "Define.h"
#include "MathUtils.h"
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
	assert(physics == nullptr && audio == nullptr);

	physics = new Physics(&physicsConfig, &bodiesConfig, &pixelsToMeters);
	audio = new Audio();

	// Pixels To Meters = [ m / pxl ]
	pixelsToMeters = pixelsPerMeter > 0 ? 1 / pixelsPerMeter : 1;

	bodyCreation = new BodyCreation(
		&bodies, 
		&gasIndex, 
		&liquidIndex, 
		physics, 
		&pixelsToMeters, 
		&bodiesConfig);

	libraryConfig = new LibraryConfig(
		&panRange,
		&physicsConfig,
		&bodiesConfig,
		&physics->globalGravity,
		&physics->globalRestitution,
		&physics->globalFriction,
		&listener,
		&pixelsToMeters,
		&ptmRatio,
		&scenarioPreset,
		&windowSize,
		&physicsPreset,
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

const BodyCreation* NPL::CreateBody(Rect rectangle)
{
	//Library not initialized. Call NPL::Init() first
	assert(physics != nullptr);

	rectangle = { rectangle.GetPosition() * pixelsToMeters, rectangle.GetSize() * pixelsToMeters };

	return bodyCreation->Access(rectangle);
}

const LibraryConfig* NPL::Configure()
{
	return libraryConfig->Access();
}

const GetData* NPL::Get()
{
	return getData->Access();
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

bool NPL::DestroyBody(BodyClass clas)
{
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
	audio->LoadSound(path);
}

void NPL::PausePhysics(bool pause)
{
	physics->globals.Set(0, pause);
}

//- Private --------------------------------------------------------------------------------

void NPL::StepPhysics(float dt)
{
	//-TOCHECK: Think about a pointer (std::vector<Body*>*) inside physics class & making for inside Step function
	for (Body* b : bodies) physics->Step(b, dt);

	physics->SolveCollisions(&bodies);

	//-TODO: Funci� per reflexar velocitat segons vaires merdes
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

		GasBody* environment = GetEnvironmentBody(b->GetRect(InUnit::IN_METERS));

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

void NPL::UpdateStates()
{
	// If debug states is false
	if (!bodiesConfig.Get(0)) return;

	for (Body* b : bodies)
	{
		if (b->clas != BodyClass::DYNAMIC_BODY) continue;
		DynamicBody* dB = (DynamicBody*)b;

		bool floating = true;
		for (int i = 1; i < 5; ++i)
		{
			if (dB->bodyState.Get(i))
			{
				floating = false;
				break;
			}
		}
		dB->bodyState.Set((int)BodyState::FLOAT, floating);

		if (IsVoid()) dB->bodyState.Set((int)BodyState::GAS, false);
		else
		{
			bool gas = false;
			for (unsigned int* i : gasIndex)
			{
				if (MathUtils::CheckCollision(b->GetRect(InUnit::IN_METERS), bodies[*i]->GetRect(InUnit::IN_METERS)))
				{
					gas = true;
					break;
				}
			}

			dB->bodyState.Set((int)BodyState::GAS, gas);
		}

		if (IsDry()) dB->bodyState.Set((int)BodyState::LIQUID, false);
		else
		{
			bool liquid = false;
			for (unsigned int* i : liquidIndex)
			{
				if (MathUtils::CheckCollision(b->GetRect(InUnit::IN_METERS), bodies[*i]->GetRect(InUnit::IN_METERS)))
				{
					liquid = true;
					break;
				}
			}

			dB->bodyState.Set((int)BodyState::LIQUID, liquid);
		}

	}
}

void NPL::ListenerLogic(Body* b, GasBody* environment)
{
	// If listener emit sound
	if (b->GetId() == listener->id)
	{
		for (AcousticData* data : b->acousticDataList)
		{
			float volume = ComputeVolume(1, data->spl);

			soundDataList.emplace_back(new SoundData(data->index, 0, volume, 0));
			RELEASE(data);
		}
		b->acousticDataList.clear();
		return;
	}

	for (AcousticData* data : b->acousticDataList)
	{
		// Get the distance between Body & Listener
		float distance = listener->GetPosition(InUnit::IN_METERS).Distance(data->position);

		float pan = ComputePanning(distance, data->position.x * pixelsToMeters);

		float volume = ComputeVolume(distance, data->spl);

		float timeDelay = ComputeTimeDelay(distance, environment);

		soundDataList.emplace_back(new SoundData(data->index, pan, volume, timeDelay));
		RELEASE(data);
	}
	b->acousticDataList.clear();
}

GasBody* NPL::GetEnvironmentBody(Rect body)
{
	for (unsigned int* index : gasIndex)
	{
		if (MathUtils::CheckCollision(body, bodies[*index]->GetRect(InUnit::IN_METERS)))
			return (GasBody*)bodies[*index];
	}

	return nullptr;
}

void NPL::UpdateNotifier()
{
	if (!notifier.IsAnyTrue()) return;

	if (notifier.Get(0)) UpdatePixelsToMeters();
	if (notifier.Get(1)) UpdateScenarioPreset();
	if (notifier.Get(2)) UpdatePhysicsPreset();

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
			dB->SetGravityOffset(dB->GetGravityOffset() * ptmRatio);
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

void NPL::UpdateScenarioPreset()
{

//-TOCHECK: Maybe should not do this \/
	DestroyBody(BodyClass::STATIC_BODY);

	switch (scenarioPreset)
	{
	case ScenarioPreset::LIMITS_SCENARIO_PRESET:
	{
		float downLimitY = windowSize.y * 0.93f;
		float rightLimitX = windowSize.x * 0.96f;
		//Limits
		CreateBody({ 0,          0,               windowSize.x, windowSize.y - downLimitY })->Static();
		CreateBody({ 0,          0, windowSize.x - rightLimitX,              windowSize.y })->Static();
		CreateBody({ 0, downLimitY,               windowSize.x, windowSize.y - downLimitY })->Static();
		CreateBody({ rightLimitX,0, windowSize.x - rightLimitX,              windowSize.x })->Static();

		break;
	}
	case ScenarioPreset::CORRIDOR_SCENARIO_PRESET:
	{
		float downLimitY = windowSize.y * 0.93f;
		float rightLimitX = windowSize.x * 0.96f;
		//Limits
		CreateBody({                  0,          0,                       4280, windowSize.y - downLimitY })->Static();
		CreateBody({                  0,          0, windowSize.x - rightLimitX,              windowSize.y })->Static();
		CreateBody({                  0, downLimitY,                       4280, windowSize.y - downLimitY })->Static();
		CreateBody({ 3000 + rightLimitX,          0, windowSize.x - rightLimitX,              windowSize.x })->Static();

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

	notifier.Set(1, false);
}

void NPL::UpdatePhysicsPreset()
{
	Point friction = {};
	Point gravity = {};
	Point restitution = {};

	switch (physicsPreset)
	{
	case PhysicsPreset::DEFAULT_PHYSICS_PRESET:
		friction = 0.2f;
		gravity = { 0.0f, 9.8f };
		restitution = { 0.1f, 0.0f };
		break;

	case PhysicsPreset::NO_PHYSIC_PRESET:
	default:
		break;
	}

	physics->globalFriction = friction;
	physics->globalGravity = gravity;
	physics->globalRestitution = restitution;

	notifier.Set(2, false);
}

float NPL::ComputePanning(float distance, float bodyX)
{
	// Check direction for audio panning (50L(neg) or 50R(pos))
	float direction = 1;
	if (listener->GetPosition(InUnit::IN_METERS).x < bodyX) direction *= -1;

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
	// Calculate delay time
	// 1. Vel = sqrt( lambda * Pa / ro )
	// 2. Time = distance / vel
	float timeDelay = 0;
	float vel = MathUtils::Sqrt(environment->GetHeatRatio() * environment->GetPressure() / environment->GetDensity());
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
