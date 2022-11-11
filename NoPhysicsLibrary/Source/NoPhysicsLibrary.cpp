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

	physics = new Physics(&bodies, &physicsConfig, &gasIndex, &liquidIndex, &pixelsToMeters);
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

const Rect NPL::ReturnScenarioRect() const
{
	Rect first = bodies.front()->GetRect(InUnit::IN_PIXELS);
	Point minP = { first.x, first.y };
	Point maxP = { first.x + first.w, first.y + first.h };

	for (Body* body : bodies)
	{
		Rect bodyRect = body->GetRect(InUnit::IN_PIXELS);
		if (bodyRect.x + bodyRect.w > maxP.x) maxP.x = bodyRect.x + bodyRect.w;
		if (bodyRect.y + bodyRect.h > maxP.y) maxP.y = bodyRect.y + bodyRect.h;
		if (bodyRect.x < minP.x) minP.x = bodyRect.x;
		if (bodyRect.y < minP.y) minP.y = bodyRect.y;
	}

	return Rect{ minP.x, minP.y, maxP.x - minP.x, maxP.y - minP.y };
}

void NPL::SetScenarioPreset(ScenarioPreset preset, Point windowSize, std::vector<StaticBody*>* scenarioBodies)
{
	switch (preset)
	{
	case ScenarioPreset::LIMITS_SCENARIO_PRESET:
	{
		float downLimitY = windowSize.y * 0.93f;
		float rightLimitX = windowSize.x * 0.96f;

		const unsigned int maxStatic = 4;
		StaticBody* scenario[maxStatic] = { nullptr, nullptr, nullptr, nullptr };
		//Limits
		scenario[0] = CreateBody({           0,          0,               windowSize.x, windowSize.y - downLimitY })->Static();
		scenario[1] = CreateBody({           0,          0, windowSize.x - rightLimitX,              windowSize.y })->Static();
		scenario[2] = CreateBody({           0, downLimitY,               windowSize.x, windowSize.y - downLimitY })->Static();
		scenario[3] = CreateBody({ rightLimitX,          0, windowSize.x - rightLimitX,              windowSize.y })->Static();

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
		CreateBody({ 0,          0,                       4280, windowSize.y - downLimitY })->Static();
		CreateBody({ 0,          0, windowSize.x - rightLimitX,              windowSize.y })->Static();
		CreateBody({ 0, downLimitY,                       4280, windowSize.y - downLimitY })->Static();
		CreateBody({ 3000 + rightLimitX,          0, windowSize.x - rightLimitX,              windowSize.x })->Static();

		if (scenarioBodies != nullptr) for (unsigned int i = 0; i < maxStatic; ++i) scenarioBodies->emplace_back(scenario[i]);

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

void NPL::SetPhysicsPreset(PhysicsPreset preset)
{
	Point friction = {};
	Point gravity = {};
	Point restitution = {};

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
	physics->Step(&bodies, dt);

	physics->SolveCollisions(&bodies);

	//-TODO: Funció per reflexar velocitat segons vaires merdes
}

void NPL::UpdateStates()
{
	//-TODONE: optimize logic to be open always

	for (Body* b : bodies)
	{
		if (b->clas != BodyClass::DYNAMIC_BODY) continue;
		DynamicBody* dB = (DynamicBody*)b;

		// Detect moving
		//-TODO: check why jumping runing left does not jump that much as to the right
		if (MathUtils::Abs(0 - MathUtils::Abs(dB->velocity.x)) > 0.001f || MathUtils::Abs(0 - MathUtils::Abs(dB->velocity.y)) > 0.001f)
			dB->bodyState.Set((int)BodyState::MOVING, true);

		// Detect floating (no collision with solids)
		bool floating = true;
		for (unsigned int i = 1; i < 5; ++i)
		{
			if (dB->IsColliding((BodyState)i))
			{
				floating = false;
				break;
			}
		}
		if (floating) dB->bodyState.Set((int)BodyState::FLOAT, true);

		// Detect liquid & Gas
		bool fullLiquidState = false;
		float totalArea = 0.0f;
		for (unsigned int* i : liquidIndex)
		{
			if (MathUtils::CheckCollision(b->GetRect(InUnit::IN_METERS), bodies[*i]->GetRect(InUnit::IN_METERS)))
			{
				dB->bodyState.Set((int)BodyState::LIQUID, true);
				totalArea += MathUtils::IntersectRectangle(b->GetRect(InUnit::IN_METERS), bodies[*i]->GetRect(InUnit::IN_METERS)).GetArea();
				fullLiquidState = (0.0001f > MathUtils::Abs(b->GetRect(InUnit::IN_METERS).GetArea() - totalArea));
				if (fullLiquidState) break;
			}
		}

		if (fullLiquidState) continue;

		for (unsigned int* i : gasIndex)
		{
			if (MathUtils::CheckCollision(b->GetRect(InUnit::IN_METERS), bodies[*i]->GetRect(InUnit::IN_METERS)))
			{
				dB->bodyState.Set((int)BodyState::GAS, true);
				break;
			}
		}
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
		float distance = listener->GetPosition(InUnit::IN_METERS).Apply(listener->emissionPoint).Distance(data->emissionPosition);

		float pan = ComputePanning(distance, data->emissionPosition.x);

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

float NPL::ComputePanning(float distance, float bodyX)
{
	// Check direction for audio panning (50L(neg) or 50R(pos))
	float direction = 1;
	if (listener->GetPosition(InUnit::IN_METERS).Apply(listener->emissionPoint).x < bodyX) direction *= -1;

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
