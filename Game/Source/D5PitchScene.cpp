#include "D5PitchScene.h"
#include "Textures.h"
#include "MathUtils.h"

D5PitchScene::D5PitchScene()
{
}

D5PitchScene::~D5PitchScene()
{
}

bool D5PitchScene::Start()
{
	physics = new NoPhysicsLibrary();
	physics->Init(60);
	physics->Configure()->PhysicsIterations(90);
	physics->SetPhysicsPreset(PhysicsPreset::DEFAULT_PHYSICS_PRESET);
	physics->Configure()->GlobalGravity({ 0, 9.8f * 3 }, InUnit::IN_METERS);
	physics->Configure()->PanRange(12, InUnit::IN_METERS);
	physics->LoadSound("Assets/Audio/bounce.wav");
	bool tr = physics->LoadSound("Assets/Audio/doorOpen.wav");
	float mTp = physics->Get()->MetersToPixels();
	physics->Configure()->SoundDelay(false);
	physics->Configure()->PitchVariationFactor(0.2, true, true);
	physics->Configure()->SoundOclusion(false);
	physics->Configure()->FrequentialAttenuation(false);

	background = texture->Load("Textures/bg_demos_01.png");
	groundTex = texture->Load("Textures/ts_demos_01.png");
	woodBoxTex = texture->Load("Textures/ts_demos_02.png");
	doorTex = texture->Load("Textures/ts_demos_03.png");
	waterTex = texture->Load("Textures/ts_demos_05.png");
	airTex = texture->Load("Textures/ts_demos_06.png");
	postTex = texture->Load("Textures/ts_demos_07.png");
	buttonTex = texture->Load("Textures/ts_demos_08.png");
	clickTex = texture->Load("Textures/txt_demos_01.png");

	door = physics->CreateBody(2940, 460, 80, 120)->Static();

	button = physics->CreateBody(1750, 500, 45, 45)->Static();

	player = physics->CreateBody(100, 380, 0.7 * mTp, 1.6 * mTp)->Dynamic(75);
	physics->Configure()->Listener(player);
	emmiter = physics->CreateBody(2390, 440, 428 / 4.5, 1440 / 4.5)->Static();
	emmiter->EmissionPoint({ -40, -140 }, InUnit::IN_PIXELS);
	player->ExcludeForCollision(door);
	player->ExcludeForCollision(emmiter);
	player->ExcludeForCollision(button);

	gas = physics->CreateBody(0, 0, 1, 1)->Gas(1.414, { 3.0f, 0.1f }, InUnit::IN_METERS);

	// Limits
	physics->CreateBody(0, 530, 1640, 230)->Static();
	physics->CreateBody(1640, 580, 1420, 230)->Static();

	physics->CreateBody(0, -500, 10, 1220)->Static();
	physics->CreateBody(3050, -200, 10, 920)->Static();

	// Roof
	physics->CreateBody(10, -180, 1640, 600)->Static();
	physics->CreateBody(1650, -350, 1400, 650)->Static();
	counter = physics->CreateBody(1840, 150, 550, 100)->Static();
	doorSoundEffectBody = physics->CreateBody(2550, 300, 510, 130)->Static();
	doorSoundEffectBody->EmissionPoint({ -250, 0 });

	blockWall = physics->CreateBody(2550, 430, 30, 150)->Static();

	Rect r = physics->ReturnScenarioRect();
	r.w -= (60 * 26);
	r.x += (60 * 26);
	r.y -= 200;
	gas->Size(r.w, r.h, InUnit::IN_PIXELS);
	gas->Position(r.x, r.y, InUnit::IN_PIXELS);
	Material mat;
	mat.BuildGas(1.414, 343 * 343, -1, 1.414);
	gas->SetMaterial(mat);

	return true;
}

bool D5PitchScene::Update(float dt)
{
	static float timer = 0;
	bool ground = player->IsBodyStill(BodyState::ON_GROUND);
	bool air = player->IsBodyStill(BodyState::FLOATING);
	bool moving = player->IsBodyStill(BodyState::MOVING);
	bool liquid = player->IsBodyStill(BodyState::IN_LIQUID);
	bool shift = (input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT);

	physics->Update(dt > 0.2 ? 0.2 : dt);

	// Camera movement
	PhysVec pos = player->Position(InUnit::IN_PIXELS);
	if (pos.x >= 425) render->camera.rect.x = pos.x - 425;
	if (pos.y <= 360) render->camera.rect.y = pos.y - 360;
	if (pos.x < 425) render->camera.rect.x = 0;
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

	if (input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_DOWN)
	{
		if (button && MathUtils::CheckCollision(button->Rect(InUnit::IN_PIXELS), player->Rect(InUnit::IN_PIXELS)))
		{
			buttonClick = true;
			buttonTexShow = true;
		}
		else if (MathUtils::CheckCollision(door->Rect(InUnit::IN_PIXELS), player->Rect(InUnit::IN_PIXELS)))
			SetScene(Scenes::INITIAL_SCENE);
	}

	if (buttonClick) PitchLogic();

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

	return true;
}

bool D5PitchScene::CleanUp()
{
	physics->CleanUp();
	delete physics;
	physics = nullptr;

	texture->UnLoad(background);
	texture->UnLoad(groundTex);
	texture->UnLoad(woodBoxTex);
	texture->UnLoad(doorTex);
	texture->UnLoad(airTex);
	texture->UnLoad(postTex);
	texture->UnLoad(buttonTex);
	texture->UnLoad(waterTex);
	texture->UnLoad(clickTex);

	buttonClick = false;
	buttonTexShow = false;

	soundTimer = 0;
	staticTimer = 0;

	return true;
}

void D5PitchScene::Draw()
{
	static float timer = 0;

	render->DrawTexture(background, { 0.f, -400 }, { 1.f, 1 }, true);
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
		{
			Point finalSize = { r.w / 1896, r.h / 1896 };
			if (b == counter)
			{
				render->DrawRectangle(r, { 0, 0, 0, 255 });
				//render->DrawTexture(woodBoxTex, b->Position(InUnit::IN_PIXELS), finalSize, true);
				break;
			}
			else if (b == emmiter)
			{
				render->DrawTexture(postTex, b->Position(InUnit::IN_PIXELS), { r.w / 428, r.h / 1440 }, true, nullptr, 0, SDL_FLIP_HORIZONTAL);
				break;
			}
			else if (b == door)
			{
				render->DrawTexture(doorTex, b->Position(InUnit::IN_PIXELS), { r.w / 360, r.h / 600 }, true);
				render->DrawTexture(woodBoxTex, b->Position(InUnit::IN_PIXELS), { r.w / 1896, r.h / 1896 }, true);
				break;
			}
			else if (b == button)
			{
				render->DrawTexture(buttonTex, b->Position(InUnit::IN_PIXELS), { r.w / 620, r.h / 620 }, true);
			}
			else
			{
				render->DrawTexture(groundTex, b->Position(InUnit::IN_PIXELS), { 1.0, 1.0 }, true, &r);
				if (r.w >= r.h && r.w / r.h < 1.8)
				{
					render->DrawTexture(woodBoxTex, b->Position(InUnit::IN_PIXELS), finalSize, true);
				}
				else if (r.h >= r.w && r.h / r.w < 1.8)
				{
					render->DrawTexture(woodBoxTex, b->Position(InUnit::IN_PIXELS), finalSize, true);
				}
				else
				{
					if (r.h > r.w)
					{
						render->DrawTexture(woodBoxTex, b->Position(InUnit::IN_PIXELS), { finalSize.x, finalSize.y / 3 }, true);
						render->DrawTexture(woodBoxTex, Point(b->Position(InUnit::IN_PIXELS)) + Point{ 0, (r.h / 3) }, { finalSize.x, finalSize.y / 3 }, true);
						render->DrawTexture(woodBoxTex, Point(b->Position(InUnit::IN_PIXELS)) + Point{ 0, (r.h / 3) * 2 }, { finalSize.x, finalSize.y / 3 }, true);
					}
					if (r.w > r.h)
					{
						render->DrawTexture(woodBoxTex, b->Position(InUnit::IN_PIXELS), { finalSize.x / 3, finalSize.y }, true);
						render->DrawTexture(woodBoxTex, Point(b->Position(InUnit::IN_PIXELS)) + Point{ (r.w / 3), 0 }, { finalSize.x / 3, finalSize.y }, true);
						render->DrawTexture(woodBoxTex, Point(b->Position(InUnit::IN_PIXELS)) + Point{ (r.w / 3) * 2, 0 }, { finalSize.x / 3, finalSize.y }, true);
					}
				}
				//render->DrawRectangle(Rect(b->EmissionPoint(InUnit::IN_PIXELS), 4, 4), { 255, 0, 0, 255 });
			}
			break;
		}
		case BodyClass::DYNAMIC_BODY:
			render->DrawRectangle(b->Rect(InUnit::IN_PIXELS), { 0, 200, 0, 220 });
			break;

		case BodyClass::GAS_BODY:
			render->DrawTexture(airTex, b->Position(InUnit::IN_PIXELS), { r.w / 5000, r.h / 5000 }, true);
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

	if (buttonClick && buttonTexShow)
	{
		render->DrawTexture(clickTex, button->Position(InUnit::IN_PIXELS) + PhysVec{ 50, 0 }, { 0.5f, 0.5f }, true);
		timer += 0.1f;
		if (timer >= 2)
		{
			timer = 0;
			buttonTexShow = false;
		}
	}
}

void D5PitchScene::PitchLogic()
{

	if (gas->Pressure() > 1000000)
	{
		buttonClick = false;
		buttonTexShow = false;
		soundTimer = 0;
		doorSoundEffectBody->Play(1, 100);
		physics->DestroyBody(blockWall);
		return;
	}

	soundTimer += 0.1;
	if (soundTimer > 2)
	{
		emmiter->Play(0, 100);
		soundTimer = 0;
	}

	staticTimer += 0.1;
	if (staticTimer < 7) return;

	Material mat;
	mat.BuildGas(gas->HeatRatio(), gas->Pressure() + 1200, -1, gas->Density(InUnit::IN_METERS));
	gas->SetMaterial(mat);
}