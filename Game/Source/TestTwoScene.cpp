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

	npl->Configure().CollisionsDebugging(false);
	npl->Configure().PanRange(10, InUnit::IN_METERS);
	npl->Configure().GlobalFriction(0.2f);
	npl->Configure().GlobalRestitution({ 0.6f, 0.0f });
	npl->Configure().GlobalGravity({ 0.0f, 3.0f }, InUnit::IN_METERS);

	//-TODO: top, ground, left right, air, water bools


	player = npl->CreateBody({ 100.0f, 200.0f, npl->MetersToPixels(Point(0.3f, 0.75f)) }, 20).Dynamic();
	npl->CreateBody({ 400.0f, 200.0f, npl->MetersToPixels(Point(0.3f, 0.75f)) }, 20).Dynamic();

	//-TODO: Add it in config?
	emitter = npl->SetScenarioPreset(ScenarioPreset::CORRIDOR_SCENARIO_PRESET, window->GetSize(), 1);

	npl->Configure().Listener(player);

	//-TODO: Insert environment enum presets, in config?
	npl->CreateBody(npl->ReturnScenarioRect(), 1).Gas(10.0f, 1.414f, 1000);

	npl->LoadSound("Assets/Audio/bounce.wav");

	render->ResetCamera();

	return true;
}

bool TestTwoScene::Update(float dt)
{
	bool ret = true;
	static bool pause = false;
	static bool bools = false;

	// Information: The user will listen from the point of view of the listener body.
	if (player->GetPosition(InUnit::IN_PIXELS).x >= 625) render->camera.x = player->GetPosition(InUnit::IN_PIXELS).x - 625;
	if (render->camera.x < 0) render->camera.x = 0;
	if (render->camera.x > 3000) render->camera.x = 3000;
	if (render->camera.y < 0) render->camera.y = 0;

	// Player Inputs
	if (input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT) player->ApplyMomentum(  20, 0);
	if (input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT) player->ApplyMomentum( -20, 0);
	if (input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN) player->ApplyMomentum(0, -300);

	if (input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_DOWN) player->Play(0, 80.0f);
	if (input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) emitter->Play(0, 80.0f);

	// Pauses the physics
	if (input->GetKey(SDL_SCANCODE_ESCAPE) == KeyState::KEY_DOWN)
	{
		pause = !pause;
		npl->PausePhysics(pause);
	}

	// Update library
	npl->Update(&dt);

	// Draw bodies
	int size = 1;
	for (int i = 0; i < size; ++i)
	{
		// Not like this
		const Body* b = npl->GetBodiesIterable(size, i);
		SDL_Color color = { 0, 0, 0, 50 };

		switch (b->GetClass())
		{
		case BodyClass::STATIC_BODY:  color = { 255,   0,   0, color.a }; break;
		case BodyClass::DYNAMIC_BODY: color = { 0, 255,   0, color.a }; break;
		case BodyClass::LIQUID_BODY:  color = { 100, 100, 255, color.a }; break;
		case BodyClass::GAS_BODY:     color = { 255, 255, 255, (Uint8)(color.a - 40) }; break;
		}

		render->DrawRectangle(b->GetRect(InUnit::IN_PIXELS), color);
	}

	// Draw the collisions
	size = 1;
	for (int i = 0; i < size; ++i)
	{
		const Collision* c = npl->GetCollisionsIterable(size, i);
		if (c) render->DrawRectangle(c->GetCollisionRectangle(), { 100, 100, 255, 255 });
	}

	// Draw collision bools
	if (input->GetKey(SDL_SCANCODE_B) == KeyState::KEY_DOWN) bools = !bools;
	if (bools)
	{
		render->DrawRectangle({ 0, 0, 50 * 3, 74 }, { 255, 255, 255, 150 }, {1.0f, 1.0f}, false);
		if (player->IsColliding(CollideBool::AIR))    render->DrawRectangle({ 10 * 3, 10, 20 * 3, 12 }, { 150, 150, 255, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(CollideBool::GROUND)) render->DrawRectangle({ 10 * 3, 32, 20 * 3, 12 }, { 255,   0,   0, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(CollideBool::LEFT))   render->DrawRectangle({ 10 * 3, 64, 20 * 3, 12 }, { 255, 255,   0, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(CollideBool::RIGHT))  render->DrawRectangle({ 40 * 3, 10, 20 * 3, 12 }, { 255,   0, 255, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(CollideBool::ROOF))   render->DrawRectangle({ 40 * 3, 32, 20 * 3, 12 }, {   0, 255,   0, 150 }, { 1.0f, 1.0f }, false);
		if (player->IsColliding(CollideBool::WATER))  render->DrawRectangle({ 40 * 3, 64, 20 * 3, 12 }, {   0,   0, 255, 150 }, { 1.0f, 1.0f }, false);
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