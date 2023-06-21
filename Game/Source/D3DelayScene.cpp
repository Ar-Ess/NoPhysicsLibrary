#include "D3DelayScene.h"
#include "Textures.h"
#include "MathUtils.h"

D3DelayScene::D3DelayScene()
{
}

D3DelayScene::~D3DelayScene()
{
}

bool D3DelayScene::Start()
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
	physics->Configure()->FrequentialAttenuation(false);
	physics->Configure()->SoundDelay(true);
	physics->Configure()->SoundOclusion(false);
	physics->Configure()->PitchVariationFactor(0, false, false);

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
	
	buttons[0] = physics->CreateBody(1550, 470, 45, 45)->Static();
	buttons[1] = physics->CreateBody(1750, 500, 45, 45)->Static();

	player = physics->CreateBody(100, 0, 0.7 * mTp, 1.6 * mTp)->Dynamic(75);
	physics->Configure()->Listener(player);
	emmiter = physics->CreateBody(2390, 470, 428 / 4.5, 1440 / 4.5)->Static();
	emmiter->EmissionPoint({-40, -140}, InUnit::IN_PIXELS);
	player->ExcludeForCollision(door);
	player->ExcludeForCollision(emmiter);
	for (int i = 0; i < 2; ++i) player->ExcludeForCollision(buttons[i]);

	gas = physics->CreateBody(0, 0, 1, 1)->Gas(0.4f, { 3.0f, 0.1f }, InUnit::IN_METERS);

	// Limits
	physics->CreateBody(   0, 530, 1275, 230)->Static();
	physics->CreateBody(1275, 555,  360, 230)->Static();
	physics->CreateBody(1635, 580, 1415, 230)->Static();

	physics->CreateBody(0, -500, 10, 1220)->Static();
	physics->CreateBody(3050, -200, 10, 920)->Static();

	// Stairs
	float sW = 150;
	float sH = 90;
	for (int stair = 0; stair < 5; ++stair) 
		physics->CreateBody(sW * stair, 150 + sH * stair, sW, 380 - sH * stair)->Static();
	
	// Roof
	physics->CreateBody(10, -380, 890, 400)->Static();
	physics->CreateBody(900,  -380, 750, 800)->Static();
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

	return true;
}

bool D3DelayScene::Update(float dt)
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
		if (stepDone && buttons[0] && MathUtils::CheckCollision(buttons[0]->Rect(InUnit::IN_PIXELS), player->Rect(InUnit::IN_PIXELS)))
		{
			buttonClick = 0;
			ChangeMaterial();
		}
		else if (buttons[1] && MathUtils::CheckCollision(buttons[1]->Rect(InUnit::IN_PIXELS), player->Rect(InUnit::IN_PIXELS)))
		{
			buttonClick = 1;
			stepDone = true;
			emmiter->Play(0, 100);
		}
		else if (MathUtils::CheckCollision(door->Rect(InUnit::IN_PIXELS), player->Rect(InUnit::IN_PIXELS)))
			SetScene(Scenes::D4_SCENE);
	}

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

	return true;
}

bool D3DelayScene::CleanUp()
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

	return true;
}

void D3DelayScene::Draw()
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
			else if (b == buttons[0] || b == buttons[1])
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
				//render->DrawRectangle(Rect(b->EmissionPoint(InUnit::IN_PIXELS), 4, 4), {255, 0, 0, 255});
			}
			break;
		}
		case BodyClass::DYNAMIC_BODY:
				render->DrawRectangle(b->Rect(InUnit::IN_PIXELS), { 0, 200, 0, 220 });
			break;

		case BodyClass::LIQUID_BODY:
			render->DrawTexture(waterTex, b->Position(InUnit::IN_PIXELS), {r.w / 300, r.h / 300}, true);
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

	if (buttonClick != -1)
	{
		render->DrawTexture(clickTex, buttons[buttonClick]->Position(InUnit::IN_PIXELS) + PhysVec{ 50, 0 }, {0.5f, 0.5f}, true);
		timer += 0.1f;
		if (timer >= 2)
		{
			timer = 0;
			buttonClick = -1;
		}
	}
}

void D3DelayScene::ChangeMaterial()
{
	switch (step)
	{
	case 0:
	{
		Material mat;
		mat.BuildGas(-1, 100, -1, 10);
		gas->SetMaterial(mat);
		break;
	}
	case 1:
	{
		Material mat;
		mat.BuildGas(-1, -1, -1, 0.4);
		gas->SetMaterial(mat);

		obstacle = physics->CreateBody(2030, 380, 180, 180)->Static();

		break;
	}
	case 2:
	{
		Material mat;
		mat.BuildSolid(0.01, -1, 100000); // Rubber
		obstacle->SetMaterial(mat);

		break;
	}
	case 3:
	{
		physics->DestroyBody(obstacle);
		obstacle = nullptr;

		obstacle = physics->CreateBody(2030, 380, 180, 180)->Liquid(400, 0.8, InUnit::IN_METERS);
		break;
	}
	case 4:
	{
		Material mat;
		mat.BuildLiquid(-1, 0.92, -1, 1000000); // Acetone
		obstacle->SetMaterial(mat);

		break;
	}
	case 5:
	{
		physics->DestroyBody(obstacle);
		physics->DestroyBody(buttons[0]);
		physics->DestroyBody(buttons[1]);
		physics->DestroyBody(blockWall);
		doorSoundEffectBody->Play(1, 100);
		break;
	}
	}
	step++;
	stepDone = false;
}