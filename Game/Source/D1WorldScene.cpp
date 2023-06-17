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
	physics->Init(256);
	physics->Configure()->PhysicsIterations(40);
	physics->SetPhysicsPreset(PhysicsPreset::DEFAULT_PHYSICS_PRESET);
	float pTm = physics->Get()->PixelsToMeters();


	background = texture->Load("Textures/bg_demos_01.png");

	player = physics->CreateBody(0.8 * pTm, 1.4 * pTm, 30, 60)->Dynamic(70);

	physics->CreateBody(   0, 710, 3060,  20)->Static();
	physics->CreateBody( -10,   0,   10, 720)->Static();
	physics->CreateBody(3060,   0,   10, 720)->Static();

	return true;
}

bool D1WorldScene::Update(float dt)
{
	bool ground = player->IsBodyStill(BodyState::ON_GROUND);
	bool shift = (input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT);

	physics->Update(dt);

	// Camera movement
	if (player->Position(InUnit::IN_PIXELS).x >= 625) render->camera.rect.x = player->Position(InUnit::IN_PIXELS).x - 625;
	if (render->camera.rect.x < 0) render->camera.rect.x = 0;
	if (render->camera.rect.x > 2560) render->camera.rect.x = 2560;
	if (render->camera.rect.y < 0) render->camera.rect.y = 0;

	// Inputs
	if (input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
	{
		player->ApplyMomentum(12, 0);
		if (ground && shift) player->ApplyForce(20, 0);
	}
	if (input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		player->ApplyMomentum(-12, 0);
		if (ground && shift) player->ApplyForce(-20, 0);
	}
	if (ground && input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		float plus = 0;
		if (shift && player->IsBodyStill(BodyState::MOVING)) plus = -45;
		player->ApplyMomentum(0, -250 + plus);
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
	render->DrawTexture(background, { 0.f, 0 }, { 1.15f, 1.15f }, true);

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