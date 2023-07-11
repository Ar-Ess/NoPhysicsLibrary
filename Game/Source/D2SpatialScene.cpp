#include "D2SpatialScene.h"
#include "Textures.h"
#include "MathUtils.h"

D2SpatialScene::D2SpatialScene()
{
}

D2SpatialScene::~D2SpatialScene()
{
}

bool D2SpatialScene::Start()
{
	physics = new NoPhysicsLibrary();
	physics->Init(60);
	physics->Configure()->PhysicsIterations(90);
	physics->SetPhysicsPreset(PhysicsPreset::DEFAULT_PHYSICS_PRESET);
	physics->Configure()->GlobalGravity({ 0, 9.8f * 3 }, InUnit::IN_METERS);
	bool tr = physics->LoadSound("Assets/Audio/bounce.wav");
	float mTp = physics->Get()->MetersToPixels();
	physics->Configure()->FrequentialAttenuation(false);
	physics->Configure()->SoundDelay(false);
	physics->Configure()->GlobalVolumeAttenuationFactor(20);
	//physics->Configure()->SoundOclusion(false);
	physics->Configure()->PitchVariationFactor(0, false, false);

	background = texture->Load("Textures/bg_demos_01.png");
	groundTex = texture->Load("Textures/ts_demos_01.png");
	woodBoxTex = texture->Load("Textures/ts_demos_02.png");
	doorTex = texture->Load("Textures/ts_demos_03.png");
	airTex = texture->Load("Textures/ts_demos_06.png");
	postTex = texture->Load("Textures/ts_demos_07.png");

	door = physics->CreateBody(2940, 220, 80, 120)->Static();

	player = physics->CreateBody(100, 380, 0.7 * mTp, 1.6 * mTp)->Dynamic(75);
	physics->Configure()->Listener(player);

	emmiter = physics->CreateBody(1900, -100, 428 / 4.5, 1440 / 4.5)->Dynamic(100);

	player->ExcludeForCollision(door);
	player->ExcludeForCollision(emmiter);

	Body* gas = physics->CreateBody(0, 0, 1, 1)->Gas(0.4f, { 3.0f, 0.1f }, InUnit::IN_METERS);

	physics->CreateBody(   0, 530,  565, 230)->Static();
	physics->CreateBody( 565, 500, 2490, 380)->Static();
	physics->CreateBody(   0, -200, 10, 920)->Static();
	physics->CreateBody(3050, -200, 10, 920)->Static();

	physics->CreateBody(2780, 420, 120,  80)->Static();
	physics->CreateBody(2900, 340, 150, 160)->Static();

	Rect r = physics->ReturnScenarioRect();
	r.w -= (60 * 9) - 30;
	r.x += (60 * 9) - 30;
	r.y -= 200;
	gas->Size(r.w, r.h, InUnit::IN_PIXELS);
	gas->Position(r.x, r.y, InUnit::IN_PIXELS);

	//INFO: This engine is not prepared for "live" audio modification. It is prepared for short/one-shot audios

	return true;
}

bool D2SpatialScene::Update(float dt)
{
	static float timer = 0;
	bool ground = player->IsBodyStill(BodyState::ON_GROUND);
	bool air = player->IsBodyStill(BodyState::FLOATING);
	bool moving = player->IsBodyStill(BodyState::MOVING);
	bool liquid = player->IsBodyStill(BodyState::IN_LIQUID);
	bool shift = (input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT);

	physics->Update(dt > 0.2 ? 0.2 : dt);

	timer += 0.1;
	if (timer > 10)
	{
		emmiter->Play(0, 100);
		timer = 0;
	}

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
		SetScene(Scenes::D3_SCENE);

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

	return true;
}

bool D2SpatialScene::CleanUp()
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

	return true;
}

void D2SpatialScene::Draw()
{
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
			if (i != 0)
			{
				Point finalSize = { r.w / 1896, r.h / 1896 };
				render->DrawTexture(groundTex, b->Position(InUnit::IN_PIXELS), { 1.0, 1.0 }, true, &r);
				if (r.w >= r.h && r.w / r.h < 2)
				{
					render->DrawTexture(woodBoxTex, b->Position(InUnit::IN_PIXELS), finalSize, true);
				}
				else if (r.h >= r.w && r.h / r.w < 2)
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
						render->DrawTexture(woodBoxTex, Point(b->Position(InUnit::IN_PIXELS)) + Point{(r.w / 3), 0 }, { finalSize.x / 3, finalSize.y }, true);
						render->DrawTexture(woodBoxTex, Point(b->Position(InUnit::IN_PIXELS)) + Point{(r.w / 3) * 2, 0 }, { finalSize.x / 3, finalSize.y }, true);
					}
				}
			}
			else
			{
				render->DrawTexture(doorTex, b->Position(InUnit::IN_PIXELS), { r.w / 360, r.h / 600 }, true);
				render->DrawTexture(woodBoxTex, b->Position(InUnit::IN_PIXELS), { r.w / 1896, r.h / 1896 }, true);
			}
			break;

		case BodyClass::DYNAMIC_BODY:
			if (b == emmiter)
			{
				render->DrawTexture(postTex, b->Position(InUnit::IN_PIXELS), { r.w / 428, r.h / 1440 }, true, nullptr, 0, player->Position(InUnit::IN_PIXELS).x > r.x ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
				break;
			}
			else
			render->DrawRectangle(b->Rect(InUnit::IN_PIXELS), { 0, 200, 0, 220 });
			break;

		case BodyClass::LIQUID_BODY:
			render->DrawRectangle(b->Rect(InUnit::IN_PIXELS), { 0, 0, 200, 220 });
			break;

		case BodyClass::GAS_BODY:
			Point p = b->Position(InUnit::IN_PIXELS);
			p.x += 50;
			render->DrawTexture(airTex, p, { r.w / 5000, r.h / 5000 }, true, nullptr, 0, SDL_FLIP_HORIZONTAL);
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
