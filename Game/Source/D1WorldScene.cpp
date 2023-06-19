#include "D1WorldScene.h"
#include "Textures.h"
#include "MathUtils.h"

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
	physics->Configure()->PhysicsIterations(90);
	physics->SetPhysicsPreset(PhysicsPreset::DEFAULT_PHYSICS_PRESET);
	physics->Configure()->GlobalGravity({0, 9.8f * 3}, InUnit::IN_METERS);
	float mTp = physics->Get()->MetersToPixels();


	background = texture->Load("Textures/bg_demos_01.png");
	groundTex  = texture->Load("Textures/ts_demos_01.png");
	woodBoxTex = texture->Load("Textures/ts_demos_02.png");
	doorTex    = texture->Load("Textures/ts_demos_03.png");
	rockTex    = texture->Load("Textures/ts_demos_04.png");

	door = physics->CreateBody(2935, 220, 80,  120)->Static();
	
	player = physics->CreateBody(100, 300, 0.7 * mTp, 1.6 * mTp)->Dynamic(75);
	//player->ForceMultiplier(3);
	
	player->ExcludeForCollision(door);

	physics->CreateBody(   0, 540, 3060, 180)->Static();
	physics->CreateBody(   0,-200,   10, 920)->Static();
	physics->CreateBody(3050,-200,   10, 920)->Static();
	physics->CreateBody( 400, 440,  121, 100)->Static();
	physics->CreateBody( 520, 340,  121, 200)->Static();
	physics->CreateBody( 640, 240,  121, 300)->Static();

	physics->CreateBody( 760, 280, 750, 260)->Liquid(400, 0.8, InUnit::IN_METERS);
	physics->CreateBody(1322, 160, 188, 121)->Liquid(700, 1.0, InUnit::IN_METERS);

	physics->CreateBody(1510, 110, 360, 430)->Static();
	physics->CreateBody(1510,  20, 500,  91)->Static();

	physics->CreateBody(1790, -50,  70,  70)->Dynamic(20);

	physics->CreateBody(2900, 340, 150, 210)->Static();
	

	return true;
}

bool D1WorldScene::Update(float dt)
{
	bool ground = player->IsBodyStill(BodyState::ON_GROUND);
	bool air = player->IsBodyStill(BodyState::FLOATING);
	bool moving = player->IsBodyStill(BodyState::MOVING);
	bool liquid = player->IsBodyStill(BodyState::IN_LIQUID);
	bool shift = (input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT);

	physics->Update(dt > 0.2 ? 0.2 : dt);

	// Camera movement
	PhysVec pos = player->Position(InUnit::IN_PIXELS);
	if (pos.x >= 625) render->camera.rect.x = pos.x - 625;
	if (pos.y <= 360) render->camera.rect.y = pos.y - 360;
	if (pos.x < 625) render->camera.rect.x = 0;
	if (pos.y > 360 + 48) render->camera.rect.y = 0;
	if (render->camera.rect.x > 1780) render->camera.rect.x = 1780;

	// Inputs
	if (input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
	{
		if (ground)
		{
			player->ApplyMomentum(40 * 3, 0);

			if (shift)
				player->ApplyMomentum(55 * 3, 0);
		}
		else if (air)
			player->ApplyMomentum(8 * 3, 0);
	}
	if (input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		if (ground)
		{
			player->ApplyMomentum(-40 * 3, 0);

			if (shift)
				player->ApplyMomentum(-55 * 3, 0);
		}
		else if (air)
			player->ApplyMomentum(-8 * 3, 0);
	}
	if (input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		if (liquid)
			player->ApplyMomentum(0, -200 * 3);
		else if (ground)
		{
			player->ApplyMomentum(0, -250 * 3);
			if (moving)
			{
				player->ApplyMomentum(0, -50 * 3);
				if (shift)	player->ApplyMomentum(0, -30 * 3);
			}
		}
	}

	Draw();

	if (input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_DOWN &&
		MathUtils::CheckCollision(door->Rect(InUnit::IN_PIXELS), player->Rect(InUnit::IN_PIXELS))) 
		SetScene(Scenes::D2_SCENE);

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
	texture->UnLoad(groundTex);
	texture->UnLoad(woodBoxTex);
	texture->UnLoad(doorTex);
	texture->UnLoad(rockTex);

	return true;
}

void D1WorldScene::Draw()
{
	render->DrawTexture(background, {    0.f, -400 }, { 1.f, 1 }, true);
	render->DrawTexture(background, { 1801.f, -400 }, { 1.f, 1 }, true);

	unsigned int size = physics->Get()->BodiesCount();
	for (unsigned int i = 0; i < size; ++i)
	{
		const Body* b = physics->Get()->Bodies(i);
		//SDL_Color color = { 0, 0, 0, 220 };

		Rect r = b->Rect(InUnit::IN_PIXELS);
		switch (b->Class())
		{
		case BodyClass::STATIC_BODY:  
			if (i != 0) render->DrawTexture(groundTex, b->Position(InUnit::IN_PIXELS), {1.0, 1.0}, true, &r);
			else render->DrawTexture(doorTex, b->Position(InUnit::IN_PIXELS), { r.w / 360, r.h / 600 }, true);
			render->DrawTexture(woodBoxTex, b->Position(InUnit::IN_PIXELS), {r.w / 1896, r.h / 1896}, true); 
			break;

		case BodyClass::DYNAMIC_BODY: 
			if (b->Id() == player->Id()) render->DrawRectangle(b->Rect(InUnit::IN_PIXELS), { 0, 200, 0, 220 });
			else render->DrawTexture(rockTex, b->Position(InUnit::IN_PIXELS), { r.w / 2600, r.h / 2600 }, true);

			break;

		case BodyClass::LIQUID_BODY: 
			render->DrawRectangle(b->Rect(InUnit::IN_PIXELS), { 0, 0, 200, 220 }); 
			break;
		}

		//switch (b->Class())
		//{
		//case BodyClass::STATIC_BODY:  color = { 200,   0,   0, color.a }; break;
		//case BodyClass::DYNAMIC_BODY: color = {   0, 200,   0, color.a }; break;
		//case BodyClass::LIQUID_BODY:  color = {   0,   0, 200, color.a }; break;
		//case BodyClass::GAS_BODY:     color = { 200, 200, 200, (Uint8)(color.a - 20) }; break;
		//}

		//render->DrawRectangle(b->Rect(InUnit::IN_PIXELS), color);
		//PhysVec v = b->EmissionPoint(InUnit::IN_PIXELS) + (-3.0f, -3);
		//render->DrawRectangle(Rect(v.x, v.y, 6, 6), { 155, 255, 155, 255 });
	}
}