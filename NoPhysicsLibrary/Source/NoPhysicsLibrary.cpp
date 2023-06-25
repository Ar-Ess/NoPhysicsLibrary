#include "NoPhysicsLibrary.h"
#include <stdint.h>
#include "Define.h"
#include "PhysMath.h"

#define INIT_CHECK() assert(physics != nullptr && audio != nullptr && acoustics != nullptr && "ERROR: NPL variable not initialized. Call Init() first.");
#define DOUBLE_INIT_CHECK() assert(physics == nullptr && audio == nullptr && acoustics == nullptr && "ERROR: NPL variable initialized twice. Call CleanUp() first before initializing the library again.");

NoPhysicsLibrary::NoPhysicsLibrary()
{
}

NoPhysicsLibrary::~NoPhysicsLibrary()
{
}

void NoPhysicsLibrary::Init(float pixelsPerMeter)
{
	DOUBLE_INIT_CHECK();
	physics = new Physics(&bodies, &generalConfig, &gasIndex, &liquidIndex, &pixelsToMeters, &physIterations);
	acoustics = new Acoustics(&bodies, &soundDataList, &gasIndex, &liquidIndex, &panRange, &panFactor, &pitchVariationFactor, &generalConfig, &volumeAttenuationFactor);
	audio = new Audio();

	notifier += std::bind(&NoPhysicsLibrary::UpdateNotifier, this, std::placeholders::_1, std::placeholders::_2);

	generalConfig.Set(0b00011110);

	// Pixels To Meters = [ m / pxl ]
	pixelsToMeters = pixelsPerMeter > 0 ? 1 / pixelsPerMeter : 1;

	bodyCreation = new BodyCreation(
		&bodies, 
		&gasIndex, 
		&liquidIndex, 
		&physics->globals, 
		&pixelsToMeters,
		&playSoundTrigger,
		&globalMultiplier
	);

	libraryConfig = new LibraryConfig(
		&panRange,
		&panFactor,
		&pitchVariationFactor,
		&generalConfig,
		&physics->globalGravity,
		&physics->globalRestitution,
		&physics->globalFriction,
		&pixelsToMeters,
		&ptmRatio,
		&physIterations,
		&notifier,
		&globalMultiplier,
		&volumeAttenuationFactor
	);

	getData = new GetData(
		&bodies, 
		&physics->collisions, 
		&generalConfig,
		&physics->globalGravity, 
		&physics->globalFriction, 
		&physics->globalRestitution, 
		&physics->globals,
		&pixelsToMeters
	);
}

void NoPhysicsLibrary::Update(const float dt)
{
	INIT_CHECK();
	if (IsGlobalPause()) return;

	//INFO: Uniform forces independent from space use InUnit::IN_PIXELS, otherwise InUnit::IN_METERS
	StepPhysics(dt);
	StepAcoustics();
	StepAudio();

	UpdateStates();
}

void NoPhysicsLibrary::CleanUp()
{
	INIT_CHECK();
	// AUDIO
	audio->CleanUp();

	// BODIES
	bodies.Clear();

	// LISTENER
	listener = nullptr;

	// PHYSICS
	physics->CleanUp();

	// SOUND DATA
	soundDataList.Clear();

	// Gas Locations
	gasIndex.Clear();

}

const BodyCreation* NoPhysicsLibrary::CreateBody(PhysRect rectangle)
{
	INIT_CHECK();
	rectangle *= pixelsToMeters;

	return bodyCreation->Access(rectangle);
}

const BodyCreation* NoPhysicsLibrary::CreateBody(float x, float y, float width, float height)
{
	INIT_CHECK();
	PhysRect rectangle = PhysRect( x, y, width, height ) * pixelsToMeters;

	return bodyCreation->Access(rectangle);
}

const LibraryConfig* NoPhysicsLibrary::Configure()
{
	INIT_CHECK();
	return libraryConfig->Access();
}

const GetData* NoPhysicsLibrary::Get()
{
	INIT_CHECK();
	return getData->Access();
}

bool NoPhysicsLibrary::DestroyBody(Body* body)
{
	INIT_CHECK();
	unsigned int index = bodies.Find(body);
	if (body == listener) listener = nullptr;
	if (body->Class() == BodyClass::LIQUID_BODY) liquidIndex.Erase(liquidIndex.Find(&index));
	else if (body->Class() == BodyClass::GAS_BODY) gasIndex.Erase(gasIndex.Find(&index));

	RestructureIndexArrays(index);

	return bodies.Erase(index);
}

bool NoPhysicsLibrary::DestroyBody(PhysID id)
{
	INIT_CHECK();
	Body find(BodyClass::EMPTY_BODY, {}, 0, nullptr);
	find.id = &id;
	unsigned int index = bodies.Find(&find);
	if (index == bodies.Find(listener)) listener = nullptr;
	if (bodies[index]->Class() == BodyClass::LIQUID_BODY) liquidIndex.Erase(liquidIndex.Find(&index));
	else if (bodies[index]->Class() == BodyClass::GAS_BODY) gasIndex.Erase(gasIndex.Find(&index));

	RestructureIndexArrays(index);

	return bodies.Erase(index);
}

bool NoPhysicsLibrary::DestroyBody(BodyClass clas)
{
	INIT_CHECK();
	bool ret = true;
	bool restructure = clas != BodyClass::LIQUID_BODY && clas != BodyClass::GAS_BODY;
	for (int i = bodies.Size(); i >= 0; --i)
	{
		Body* b = bodies[i];
		if (b->Class() == clas)
		{
			if (b == listener) listener = nullptr;
			bool destroyed = bodies.Erase(i);
			if (restructure) RestructureIndexArrays(i);
			if (!destroyed) ret = false;
		}
	}

	if (clas == BodyClass::GAS_BODY) gasIndex.Clear();
	else if (clas == BodyClass::LIQUID_BODY) liquidIndex.Clear();

	return ret;
}

bool NoPhysicsLibrary::LoadSound(const char* path)
{
	INIT_CHECK();
	return audio->LoadSound(path);
}

void NoPhysicsLibrary::PausePhysics(bool pause)
{
	INIT_CHECK();
	physics->globals.Set(0, pause);
}

const PhysRect NoPhysicsLibrary::ReturnScenarioRect()
{
	INIT_CHECK();
	if (bodies.Empty()) return PhysRect();

	PhysRect first = bodies.At(0)->Rect(InUnit::IN_PIXELS);
	PhysVec minP = { first.x, first.y };
	PhysVec maxP = { first.x + first.w, first.y + first.h };

	bodies.Iterate<PhysVec&, PhysVec&>
	(
		[](Body* b, PhysVec& maxP, PhysVec& minP)
		{
			PhysRect bodyRect = b->Rect(InUnit::IN_PIXELS);
			if (bodyRect.x + bodyRect.w > maxP.x) maxP.x = bodyRect.x + bodyRect.w;
			if (bodyRect.y + bodyRect.h > maxP.y) maxP.y = bodyRect.y + bodyRect.h;
			if (bodyRect.x < minP.x) minP.x = bodyRect.x;
			if (bodyRect.y < minP.y) minP.y = bodyRect.y;
		},
		maxP, minP
	);

	/*for(unsigned int i = 0; i < bodies.Size(); ++i)
	{
		Body* body = bodies[i];
		PhysRect bodyRect = body->Rect(InUnit::IN_PIXELS);
		if (bodyRect.x + bodyRect.w > maxP.x) maxP.x = bodyRect.x + bodyRect.w;
		if (bodyRect.y + bodyRect.h > maxP.y) maxP.y = bodyRect.y + bodyRect.h;
		if (bodyRect.x < minP.x) minP.x = bodyRect.x;
		if (bodyRect.y < minP.y) minP.y = bodyRect.y;
	}*/

	return PhysRect{ minP.x, minP.y, maxP.x - minP.x, maxP.y - minP.y };
}

void NoPhysicsLibrary::SetScenarioPreset(ScenarioPreset preset, PhysVec windowSize, std::vector<StaticBody*>* scenarioBodies)
{
	INIT_CHECK();
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
		scenario[0] = CreateBody({ 0.f,          0.f,                       4280.f, windowSize.y - downLimitY })->Static();
		scenario[1] = CreateBody({ 0.f,          0.f, windowSize.x - rightLimitX,              windowSize.y })->Static();
		scenario[2] = CreateBody({ 0.f, downLimitY,                       4280.f, windowSize.y - downLimitY })->Static();
		scenario[3] = CreateBody({ 3000 + rightLimitX,          0.f, windowSize.x - rightLimitX,              windowSize.x })->Static();

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
		scenario[4] = CreateBody({ windowSize.x * 0.23f, windowSize.y * 0.27f, windowSize.x * 0.02f, windowSize.y * 0.55f })->Static();
		scenario[5] = CreateBody({ windowSize.x * 0.40f, windowSize.y * 0.27f, windowSize.x * 0.02f, windowSize.y * 0.55f })->Static();
		scenario[6] = CreateBody({ windowSize.x * 0.61f, windowSize.y * 0.27f, windowSize.x * 0.02f, windowSize.y * 0.55f })->Static();
		scenario[7] = CreateBody({ windowSize.x * 0.84f, windowSize.y * 0.27f, windowSize.x * 0.02f, windowSize.y * 0.55f })->Static();
		break;
	}
	}
}

void NoPhysicsLibrary::SetPhysicsPreset(PhysicsPreset preset)
{
	INIT_CHECK();
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

void NoPhysicsLibrary::UpdateNotifier(unsigned int notify, PhysID id)
{
	switch (notify)
	{
	case 0: UpdatePixelsToMeters(); break;
	case 1: UpdateListener(id); break;
	default: assert(false && "NPL ERROR: 'UpdateNotifier' notified an invalid change out of bounds"); break;
	}
}

void NoPhysicsLibrary::UpdatePixelsToMeters()
{
	bodies.Iterate<float>
	(
		[](Body* b, float ptmRatio)
		{
			switch (b->Class())
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
		},
		ptmRatio
	);

	panRange *= ptmRatio;
	physics->globalGravity *= ptmRatio;
}

void NoPhysicsLibrary::UpdateListener(PhysID id)
{
	Body find(BodyClass::EMPTY_BODY, {}, 0, nullptr);
	find.id = &id;
	listener = bodies[bodies.Find(&find)];
}

void NoPhysicsLibrary::UpdateStates()
{
	for (unsigned int i = 0; i < bodies.Size(); ++i)
	{
		Body* b = bodies[i];
		if (b->clas != BodyClass::DYNAMIC_BODY) continue;
		DynamicBody* dB = (DynamicBody*)b;

		// Detect moving
		if (PhysMath::Abs(dB->velocity.x) > 0.01f || PhysMath::Abs(dB->velocity.y) > 0.01f)
		{
			if (!dB->prevBodyState.Get((int)BodyState::MOVING)) dB->bodyStateEnter.Set((int)BodyState::MOVING, true);
			dB->bodyStateStay.Set((int)BodyState::MOVING, true);
		}

		// Detect floating (no collision with solids)
		bool floating = true;

		for (unsigned int i = 1; i < 5; ++i)
		{
			if ((BodyState)i == BodyState::MOVING) continue;

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
		for (unsigned int index = 0; index < liquidIndex.Size(); ++index)
		{
			unsigned int* i = liquidIndex[index];
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
			for (unsigned int index = 0; index < gasIndex.Size(); ++index)
			{
				unsigned int* i = gasIndex[index];
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

void NoPhysicsLibrary::StepPhysics(const float dt)
{
	bodies.Iterate<Physics*, float>
	(
		[](Body* b, Physics* p, float dt) 
		{ 
			p->Step(b, dt); 
		},
		physics, dt
	);

	physics->SolveCollisions(&bodies);
}

void NoPhysicsLibrary::StepAcoustics()
{
	if (playSoundTrigger()) return;

	// If no listener & environment is void
	if (!listener && IsVoid()) return bodies.Iterate([](Body* b) { b->acousticDataList.Clear(); });

	bodies.Iterate<Acoustics*, Body*>
	(
		[](Body* b, Acoustics* acoustics, Body* listener)
		{
			if (b->acousticDataList.Empty() || !b->HasAcousticsUpdatability()) return;
			acoustics->Simulate(b, listener);
		},
		acoustics, listener
	);
}

void NoPhysicsLibrary::StepAudio()
{
	if (soundDataList.Empty()) return;

	soundDataList.Iterate<Audio*>
		(
			[](SoundData* data, Audio* audio)
			{
				audio->Playback(data);
			},
			audio
		);

	soundDataList.Clear();
}

void NoPhysicsLibrary::RestructureIndexArrays(unsigned int index)
{
	liquidIndex.Iterate<unsigned int>
	(
		[](unsigned int* bodyIndex, unsigned int index)
		{
			if (*bodyIndex < index) return;

			*bodyIndex = *bodyIndex - 1;
		},
		index
	);

	gasIndex.Iterate<unsigned int>
	(
		[](unsigned int* bodyIndex, unsigned int index)
		{
			if (*bodyIndex < index) return;

			*bodyIndex = *bodyIndex - 1;
		},
		index
	);
}
