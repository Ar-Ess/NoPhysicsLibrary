#include "D1WorldScene.h"
#include "Textures.h"

D1WorldScene::D1WorldScene()
{
}

D1WorldScene::~D1WorldScene()
{
}

bool D1WorldScene::Start()
{
	physics = new NoPhysicsLibrary();
	physics->Init(60);
	physics->Configure()->PhysicsIterations(40);
	physics->SetPhysicsPreset(PhysicsPreset::DEFAULT_PHYSICS_PRESET);
	float mTp = physics->Get()->MetersToPixels();


	background = texture->Load("Textures/bg_demos_01.png");

	player = physics->CreateBody(100, 300, 0.7 * mTp, 1.6 * mTp)->Dynamic(70);
	player->ForceMultiplier(3);

	physics->CreateBody(   0, 710, 3060,  20)->Static();
	physics->CreateBody(   0,   0,   10, 720)->Static();
	physics->CreateBody(3050,   0,   10, 720)->Static();

	return true;
}

bool D1WorldScene::Update(float dt)
{
	bool ground = player->IsBodyStill(BodyState::ON_GROUND);
	bool air = player->IsBodyStill(BodyState::FLOATING);
	bool moving = player->IsBodyStill(BodyState::MOVING);
	bool shift = (input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT);

	physics->Update(dt > 0.2 ? 0.2 : dt);

	// Camera movement
	PhysVec pos = player->Position(InUnit::IN_PIXELS);
	if (pos.x >= 625) render->camera.rect.x = pos.x - 625;
	if (pos.x < 625) render->camera.rect.x = 0;
	if (render->camera.rect.x > 1780) render->camera.rect.x = 1780;
	if (render->camera.rect.y < 0) render->camera.rect.y = 0;

	// Inputs
	if (input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
	{
		if (ground)
		{
			player->ApplyMomentum(40, 0);

			if (shift)
				player->ApplyMomentum(55, 0);
		}
		else if (air)
			player->ApplyMomentum(8, 0);
	}
	if (input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		if (ground)
		{
			player->ApplyMomentum(-40, 0);

			if (shift)
				player->ApplyMomentum(-55, 0);
		}
		else if (air)
			player->ApplyMomentum(-8, 0);
	}
	if (ground && input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		player->ApplyMomentum(0, -250);
		if (moving)
		{
			player->ApplyMomentum(0, -50);
			if (shift)	player->ApplyMomentum(0, -30);

		}
	}

	Draw();

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

	return true;
}

bool D1WorldScene::CleanUp()
{
	physics->CleanUp();
	delete physics;
	physics = nullptr;

	texture->UnLoad(background);


	return true;
}

void D1WorldScene::Draw()
{
	render->DrawTexture(background, {    0.f, 0 }, { 1.f, 1 }, true);
	render->DrawTexture(background, { 1801.f, 0 }, { 1.f, 1 }, true);

	unsigned int size = physics->Get()->BodiesCount();
	for (unsigned int i = 0; i < size; ++i)
	{
		const Body* b = physics->Get()->Bodies(i);
		SDL_Color color = { 0, 0, 0, 220 };

		switch (b->Class())
		{
		case BodyClass::STATIC_BODY:  color = { 200,   0,   0, color.a }; break;
		case BodyClass::DYNAMIC_BODY: color = {   0, 200,   0, color.a }; break;
		case BodyClass::LIQUID_BODY:  color = {   0,   0, 200, color.a }; break;
		case BodyClass::GAS_BODY:     color = { 200, 200, 200, (Uint8)(color.a - 20) }; break;
		}
		render->DrawRectangle(b->Rect(InUnit::IN_PIXELS), color);
		PhysVec v = b->EmissionPoint(InUnit::IN_PIXELS) + (-3.0f, -3);
		render->DrawRectangle(Rect(v.x, v.y, 6, 6), { 155, 255, 155, 255 });
	}
}