#include "TestThreeScene.h"

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
	npl->Configure()->PanRange(10, InUnit::IN_METERS);
	npl->SetPhysicsPreset(PhysicsPreset::DEFAULT_PHYSICS_PRESET);
	npl->SetScenarioPreset(ScenarioPreset::LIMITS_SCENARIO_PRESET, window->GetSize());
	npl->Configure()->Listener(player);
	npl->LoadSound("Assets/Audio/bounce.wav");

	// Create bodies
	// Dynamic
	player = npl->CreateBody({ 100.0f, 200.0f, Point(metersToPixels * 0.3f, metersToPixels * 0.75f) })
		->Dynamic(80);

	// Static
	npl->CreateBody({ 600, 500, 50, 10 })
		->Static();
	npl->CreateBody({ 800, 300, 50, 10 })
		->Static();

	// Liquid
	npl->CreateBody({ 900, 400, Point(metersToPixels * 2.6f, metersToPixels * 2.1f) })
		->Liquid(997, 0.8f, InUnit::IN_METERS);

	// Gas
	npl->CreateBody(npl->ReturnScenarioRect())
		->Gas(1500, 1.414f, 1000, { 1.5f, 0.1f }, InUnit::IN_METERS);

	// Camera
	render->ResetCamera();

	return true;
}

bool TestThreeScene::Update(float dt)
{
	// Variables
	static bool pause = false;
	static bool state = false;
	bool ground = player->IsColliding(BodyState::GROUND);
	bool shift = (input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT);

	// Camera movement
	if (player->GetPosition(InUnit::IN_PIXELS).x >= 625) render->camera.x = player->GetPosition(InUnit::IN_PIXELS).x - 625;
	if (render->camera.x < 0) render->camera.x = 0;
	if (render->camera.x > 3000) render->camera.x = 3000;
	if (render->camera.y < 0) render->camera.y = 0;

	// Inputs
	if (input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
	{
		player->ApplyMomentum(50, 0);
		if (ground && shift) player->ApplyForce(50, 0);
	}

	if (input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		player->ApplyMomentum(-50, 0);
		if (ground && shift) player->ApplyForce(-50, 0);
	}

	if (ground && input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		float plus = 0;
		if (shift && player->IsColliding(BodyState::MOVING))
		{
			plus = -65;
		}
		player->ApplyMomentum(0, -550 + plus);
	}

	// Functionalities
	// Reset forces
	if (input->GetKey(SDL_SCANCODE_C) == KeyState::KEY_DOWN) player->ResetForces();

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
	static bool change = true;
	for (int i = 0; i < size; ++i)
	{
		// Not like this
		const Body* b = npl->Get()->Bodies(i);
		SDL_Color color = { 0, 0, 0, 50 };

		switch (b->GetClass())
		{
		case BodyClass::STATIC_BODY:  color = { 255,   0,   0, color.a }; break;
		case BodyClass::DYNAMIC_BODY: color = { 0, 255,   0, color.a }; break;
		case BodyClass::LIQUID_BODY:  color = { 100, 100, 255, color.a }; break;
		case BodyClass::GAS_BODY:     color = { 255, 255, 255, (Uint8)(color.a - 20) }; break;
		}

		render->DrawRectangle(b->GetRect(InUnit::IN_PIXELS), color);
		render->DrawRectangle(Rect{ b->GetEmissionPoint(InUnit::IN_PIXELS).Apply({-3.0f, -3}), 6, 6 }, { 155, 255, 155, 255 });
	}

	// Draw the collisions
	size = npl->Get()->CollisionsCount();
	for (int i = 0; i < size; ++i)
	{
		const Collision* c = npl->Get()->Collisions(i);
		if (c) render->DrawRectangle(c->GetCollisionRectangle(InUnit::IN_PIXELS), { 100, 100, 255, 255 });
	}

	// Draw body states
	if (input->GetKey(SDL_SCANCODE_B) == KeyState::KEY_DOWN) state = !state;
	if (state)
	{
		render->DrawRectangle({ 0, 0, 120, 88 }, { 255, 255, 255, 150 }, { 1.0f, 1.0f }, false);
		if (ground)                                 render->DrawRectangle({ 20, 10, 30, 10 }, { 255,   0,   0, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(BodyState::LEFT))   render->DrawRectangle({ 20, 30, 30, 10 }, { 255, 255,   0, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(BodyState::RIGHT))  render->DrawRectangle({ 20, 50, 30, 10 }, { 255,   0, 255, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(BodyState::ROOF))   render->DrawRectangle({ 20, 70, 30, 10 }, { 0, 255,   0, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(BodyState::GAS))    render->DrawRectangle({ 70, 10, 30, 10 }, { 150, 150, 255, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(BodyState::LIQUID)) render->DrawRectangle({ 70, 30, 30, 10 }, { 0,   0, 255, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(BodyState::FLOAT))  render->DrawRectangle({ 70, 50, 30, 10 }, { 100, 100, 100, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(BodyState::MOVING)) render->DrawRectangle({ 70, 70, 30, 10 }, { 50, 200,  50, 150 }, { 1.0f, 1.0f }, false);
		//-TODO: Change name of function by something like "GetBodyState()" os "IsBody()" <. best option
	}

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

	return true;
}

bool TestThreeScene::CleanUp()
{
	if (!npl)
	{
		npl->CleanUp();
		RELEASE(npl);
	}

	player = nullptr;
	emitter = nullptr;
	return true;
}