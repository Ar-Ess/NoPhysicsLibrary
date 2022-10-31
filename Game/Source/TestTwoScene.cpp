#include "TestTwoScene.h"

TestTwoScene::TestTwoScene()
{
}

TestTwoScene::~TestTwoScene()
{
}

bool TestTwoScene::Start()
{
	npl = new NPL();
	npl->Init(128.0f);

	npl->Configure()->CollisionsDebugging(true);
	npl->Configure()->PanRange(10, InUnit::IN_METERS);
	npl->SetPhysicsPreset(PhysicsPreset::DEFAULT_PHYSICS_PRESET);
	npl->SetScenarioPreset(ScenarioPreset::LIMITS_SCENARIO_PRESET, window->GetSize());
	float mTP = npl->Get()->MetersToPixels();

	player = npl->CreateBody({ 100.0f, 200.0f, Point(mTP * 0.3f, mTP * 0.75f) })->Dynamic(12);
	//npl->CreateBody({ 400.0f, 200.0f, Point(mTP * 0.3f, mTP * 0.75f) })->Dynamic(12);
	npl->CreateBody(npl->ReturnScenarioRect())->Gas(10, 1.414f, 1000, InUnit::IN_METERS);
	npl->CreateBody({ 400, 400, Point(mTP * 1.0f, mTP * 6.0f) })->Liquid(50, 1)->SetCollidable(false);
	npl->CreateBody({ 400 + (mTP * 1.0f), 400, Point(mTP * 1.0f, mTP * 6.0f) })->Liquid(50, 1);

	npl->Configure()->Listener(player);

	npl->LoadSound("Assets/Audio/bounce.wav");

	render->ResetCamera();

	return true;
}

bool TestTwoScene::Update(float dt)
{
	bool ret = true;
	static bool pause = false;
	static bool state = false;

	// Information: The user will listen from the point of view of the listener body.
	if (player->GetPosition(InUnit::IN_PIXELS).x >= 625) render->camera.x = player->GetPosition(InUnit::IN_PIXELS).x - 625;
	if (render->camera.x < 0) render->camera.x = 0;
	if (render->camera.x > 3000) render->camera.x = 3000;
	if (render->camera.y < 0) render->camera.y = 0;

	// Player Inputs
	if (input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT) player->ApplyMomentum(  10,   0);
	if (input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT) player->ApplyMomentum( -10,   0);
	if (player->IsColliding(BodyState::GROUND) && input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN) player->ApplyMomentum( 0, -50);
	if (input->GetKey(SDL_SCANCODE_C) == KeyState::KEY_DOWN) player->ResetForces();

	if (input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_DOWN) player->Play(0, 80.0f);
	//if (input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) emitter->Play(0, 80.0f);

	// Pauses the physics
	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KeyState::KEY_DOWN)
	{
		pause = !pause;
		npl->PausePhysics(pause);
	}

	// Update library
	npl->Update(&dt);

	// Draw bodies
	unsigned int size = npl->Get()->BodiesCount();
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
		render->DrawRectangle({ 0, 0, 120, 88 }, { 255, 255, 255, 150 }, {1.0f, 1.0f}, false);
		if (player->IsColliding(BodyState::GROUND)) render->DrawRectangle({  20, 10, 30, 10 }, { 255,   0,   0, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(BodyState::LEFT))   render->DrawRectangle({  20, 30, 30, 10 }, { 255, 255,   0, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(BodyState::RIGHT))  render->DrawRectangle({  20, 50, 30, 10 }, { 255,   0, 255, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(BodyState::ROOF))   render->DrawRectangle({  20, 70, 30, 10 }, {   0, 255,   0, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(BodyState::GAS))    render->DrawRectangle({  70, 10, 30, 10 }, { 150, 150, 255, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(BodyState::LIQUID)) render->DrawRectangle({  70, 30, 30, 10 }, {   0,   0, 255, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(BodyState::FLOAT))  render->DrawRectangle({  70, 50, 30, 10 }, { 100, 100, 100, 150 }, { 1.0f, 1.0f }, false);
	}

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

	return ret;
}

bool TestTwoScene::CleanUp()
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