#include "TestThreeScene.h"
#include "MathUtils.h"

TestThreeScene::TestThreeScene()
{
}

TestThreeScene::~TestThreeScene()
{
}

bool TestThreeScene::Start()
{
	// Initialize Variables
	npl = new NPL();
	npl->Init(128);
	float metersToPixels = npl->Get()->MetersToPixels();

	// NPL configuration
	npl->Configure()->CollisionsDebugging(true);
	npl->Configure()->PanRange(7, InUnit::IN_METERS);
	npl->Configure()->PanFactor(1.2f);
	npl->Configure()->PhysicsIterations(40);
	npl->SetPhysicsPreset(PhysicsPreset::DEFAULT_PHYSICS_PRESET);
	std::vector<StaticBody*> x;
	npl->SetScenarioPreset(ScenarioPreset::CORRIDOR_SCENARIO_PRESET, PhysVec(window->GetSize().x, window->GetSize().y), &x);
	npl->LoadSound("Assets/Audio/bounce.wav");

	// Create bodies

	// Dynamic
	player = npl->CreateBody(PhysRect( 100.0f, 200.0f, PhysVec(metersToPixels * 0.3f, metersToPixels * 0.75f) ))
		->Dynamic(80);

	npl->Configure()->Listener(player);
	//player->EmissionPoint(PhysVec(-5.0f, 0), InUnit::IN_PIXELS);

	npl->CreateBody(PhysRect( 150.0f, 400.0f, PhysVec(metersToPixels * 0.3f, metersToPixels * 0.75f) ))->Dynamic(80);

	// Static
	npl->CreateBody({ 600, 520, 50, 10 })
		->Static();
	npl->CreateBody({ 800, 360, 50, 10 })
		->Static();
	StaticBody* bouncyBoy = npl->CreateBody({ 200, 600, 50, 50 })
		->Static();
	bouncyBoy->RestitutionOffset({0.0f, 1.2f});
	x.at(0)->RestitutionOffset({0.0f, 1.2f});
	
	// Shell Obstacles
	npl->CreateBody({ 1300, 600, 30, 60 })
		->Static();
	npl->CreateBody({ 2000, 600, 30, 60 })
		->Static();

	// Liquid
	npl->CreateBody(PhysRect{ 900, 400, PhysVec(metersToPixels * 2.6f, metersToPixels * 2.1f) })
		->Liquid(997, 1.0f, InUnit::IN_METERS);
	
	//npl->CreateBody({ 1600, 550, 90, 120 })
	//	->Liquid(400, 1.0f, InUnit::IN_METERS);

	// Gas
	const GasBody* gas = npl->CreateBody(npl->ReturnScenarioRect())
		->Gas(1, 5000, { 3.0f, 0.1f }, InUnit::IN_METERS, InUnit::IN_METERS);

	// Shell
	// TODO: debug
	shell = npl->CreateBody({ 1900, 500, 50, 50 })
		->Dynamic(40);
	shell->ApplyMomentum({ -200.0f, 0 });
	shell->FrictionOffset({ -0.5f, 0.0f });
	shell->RestitutionOffset({ 1.0f, 0.0f });
	shell->ExcludeForCollision(gas);
	shell->HasPhysicsUpdatability(false);

	// Camera
	render->ResetCamera();

	return true;
}

bool TestThreeScene::Update(float dt)
{
	// Variables
	static bool pause = false;
	static bool state = false;
	bool ground = player->IsBodyStill(BodyState::ON_GROUND);
	bool shift = (input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT);

	// Camera movement
	if (player->Position(InUnit::IN_PIXELS).x >= 625) render->camera.rect.x = player->Position(InUnit::IN_PIXELS).x - 625;
	if (render->camera.rect.x < 0) render->camera.rect.x = 0;
	if (render->camera.rect.x > 3000) render->camera.rect.x = 3000;
	if (render->camera.rect.y < 0) render->camera.rect.y = 0;

	if (MathUtils::CheckCollision(shell->Rect(InUnit::IN_PIXELS), render->camera.rect)) shell->HasPhysicsUpdatability(true);
	else shell->HasPhysicsUpdatability(false);

	// Inputs
	if (input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
	{
		player->ApplyMomentum(20, 0);
		if (ground && shift) player->ApplyForce(30, 0);
	}

	if (input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		player->ApplyMomentum(-20, 0);
		if (ground && shift) player->ApplyForce(-30, 0);
	}

	if (ground && input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		float plus = 0;
		if (shift && player->IsBodyStill(BodyState::MOVING))
		{
			plus = -65;
		}
		player->ApplyMomentum(0, -400 + plus);
	}

	// Functionalities
	// Reset forces
	if (input->GetKey(SDL_SCANCODE_C) == KeyState::KEY_DOWN) player->ResetForces();

	if (input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
		shell->Play(0);

	// Pauses the physics
	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KeyState::KEY_DOWN)
	{
		pause = !pause;
		npl->PausePhysics(pause);
	}

	// Update library
	npl->Update(&dt);

	// Draw Library
	// Draw bodies
	unsigned int size = npl->Get()->BodiesCount();
	for (unsigned int i = 0; i < size; ++i)
	{
		const Body* b = npl->Get()->Bodies(i);
		SDL_Color color = { 0, 0, 0, 50 };

		switch (b->Class())
		{
		case BodyClass::STATIC_BODY:  color = { 255,   0,   0, color.a }; break;
		case BodyClass::DYNAMIC_BODY: color = { 0, 255,   0, color.a }; break;
		case BodyClass::LIQUID_BODY:  color = { 100, 100, 255, color.a }; break;
		case BodyClass::GAS_BODY:     color = { 255, 255, 255, (Uint8)(color.a - 20) }; break;
		}
		render->DrawRectangle(b->Rect(InUnit::IN_PIXELS), color);
		PhysVec v = b->EmissionPoint(InUnit::IN_PIXELS) + (-3.0f, -3);
		render->DrawRectangle(Rect( v.x, v.y, 6, 6 ), { 155, 255, 155, 255 });
	}

	// Draw the collisions
	size = npl->Get()->CollisionsCount();
	for (unsigned int i = 0; i < size; ++i)
	{
		const Collision* c = npl->Get()->Collisions(i);
		if (c) render->DrawRectangle(c->GetCollisionRectangle(InUnit::IN_PIXELS), { 100, 100, 255, 255 });
	}

	// Draw body states
	if (input->GetKey(SDL_SCANCODE_B) == KeyState::KEY_DOWN) state = !state;
	if (state)
	{
		render->DrawRectangle({ 0, 0, 120, 88 }, { 255, 255, 255, 150 }, {0.0f, 0.0f}, { 1.0f, 1.0f }, false);
		if (ground)                               render->DrawRectangle({ 20, 10, 30, 10 }, { 255,   0,   0, 150 }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyStill(BodyState::ON_LEFT  )) render->DrawRectangle({ 20, 30, 30, 10 }, { 255, 255,   0, 150 }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyStill(BodyState::ON_RIGHT )) render->DrawRectangle({ 20, 50, 30, 10 }, { 255,   0, 255, 150 }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyStill(BodyState::ON_ROOF  )) render->DrawRectangle({ 20, 70, 30, 10 }, {   0, 255,   0, 150 }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyStill(BodyState::IN_GAS   )) render->DrawRectangle({ 70, 10, 30, 10 }, { 150, 150, 255, 150 }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyStill(BodyState::IN_LIQUID)) render->DrawRectangle({ 70, 30, 30, 10 }, {   0,   0, 255, 150 }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyStill(BodyState::FLOATING )) render->DrawRectangle({ 70, 50, 30, 10 }, { 100, 100, 100, 150 }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyStill(BodyState::MOVING   )) render->DrawRectangle({ 70, 70, 30, 10 }, {  50, 200,  50, 150 }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, false);

		render->DrawRectangle({ 0, 0, 120, 88 }, { 255, 255, 255, 150 }, { 130.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyEnter(BodyState::ON_GROUND)) render->DrawRectangle({ 20, 10, 30, 10 }, { 255,   0,   0, 150 }, { 130.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyEnter(BodyState::ON_LEFT  )) render->DrawRectangle({ 20, 30, 30, 10 }, { 255, 255,   0, 150 }, { 130.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyEnter(BodyState::ON_RIGHT )) render->DrawRectangle({ 20, 50, 30, 10 }, { 255,   0, 255, 150 }, { 130.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyEnter(BodyState::ON_ROOF  )) render->DrawRectangle({ 20, 70, 30, 10 }, { 0, 255,   0, 150 },   { 130.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyEnter(BodyState::IN_GAS   )) render->DrawRectangle({ 70, 10, 30, 10 }, { 150, 150, 255, 150 }, { 130.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyEnter(BodyState::IN_LIQUID)) render->DrawRectangle({ 70, 30, 30, 10 }, { 0,   0, 255, 150 },   { 130.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyEnter(BodyState::FLOATING )) render->DrawRectangle({ 70, 50, 30, 10 }, { 100, 100, 100, 150 }, { 130.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyEnter(BodyState::MOVING   )) render->DrawRectangle({ 70, 70, 30, 10 }, { 50, 200,  50, 150 },  { 130.0f, 0.0f }, { 1.0f, 1.0f }, false);

		render->DrawRectangle({ 0, 0, 120, 88 }, { 255, 255, 255, 150 }, { 260.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyExit(BodyState::ON_GROUND)) render->DrawRectangle({ 20, 10, 30, 10 }, { 255,   0,   0, 150 }, { 260.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyExit(BodyState::ON_LEFT)) render->DrawRectangle({ 20, 30, 30, 10 }, { 255, 255,   0, 150 },   { 260.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyExit(BodyState::ON_RIGHT)) render->DrawRectangle({ 20, 50, 30, 10 }, { 255,   0, 255, 150 },  { 260.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyExit(BodyState::ON_ROOF)) render->DrawRectangle({ 20, 70, 30, 10 }, { 0, 255,   0, 150 },     { 260.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyExit(BodyState::IN_GAS)) render->DrawRectangle({ 70, 10, 30, 10 }, { 150, 150, 255, 150 },    { 260.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyExit(BodyState::IN_LIQUID)) render->DrawRectangle({ 70, 30, 30, 10 }, { 0,   0, 255, 150 },   { 260.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyExit(BodyState::FLOATING)) render->DrawRectangle({ 70, 50, 30, 10 }, { 100, 100, 100, 150 },  { 260.0f, 0.0f }, { 1.0f, 1.0f }, false);
		if (player->IsBodyExit(BodyState::MOVING)) render->DrawRectangle({ 70, 70, 30, 10 }, { 50, 200,  50, 150 },     { 260.0f, 0.0f }, { 1.0f, 1.0f }, false);
	}

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

	return true;
}

bool TestThreeScene::CleanUp()
{
	if (npl)
	{
		npl->CleanUp();
		RELEASE(npl);
	}

	player = nullptr;
	emitter = nullptr;
	return true;
}
