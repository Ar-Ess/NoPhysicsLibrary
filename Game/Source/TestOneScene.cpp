#include "TestOneScene.h"
#include "Define.h"

TestOneScene::TestOneScene()
{
}

TestOneScene::~TestOneScene()
{
	CleanUp();
}

bool TestOneScene::Start()
{
	// Initialize Library
	npl = new NPL();
	npl->Init();

	npl->Configure().CollisionsDebugging(false);

	// Get Body
	bodies = npl->TempGetBodiesDebug();
	collisions = npl->GetCollisionsIterable();

	npl->SetScenarioPreset(ScenarioPreset::CORRIDOR_SCENARIO_PRESET, window->GetSize());
	emiter1 = (StaticBody*)bodies->at(1);
	emiter2 = (StaticBody*)bodies->back();
	npl->CreateBody(Rect{ 150, 350, 200, 35 }, 1).Static();
	test = (DynamicBody*)npl->CreateBody(Rect{ 230, 100, 50, 80 }, 1).Dynamic();
	npl->CreateBody(npl->ReturnScenarioRect(), 1).Gas(10, 1.414f, 1000);

	npl->SetListener(test);
	npl->SetGlobalGravity({ 0, 400 });

	npl->LoadSound("Assets/Audio/bounce.wav");

	return true;
}

bool TestOneScene::Update(float dt)
{
	bool ret = true;
	static bool pause = false;

	// Information: The user will listen from the point of view of the listener body.
	if (test->GetPosition().x >= 625) render->camera.SetPosition(test->GetPosition().Apply({ -625, -550 }).Multiply({-1, -1}));
	if (render->camera.x > 0) render->camera.x = 0;
	if (-render->camera.x > 3000) render->camera.x = -3000;

	// Update Dynamic Body
	if (input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT) test->ApplyForce(-100, 0);
	if (input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT) test->ApplyForce(100, 0);
	if (input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) test->Play(0);
	if (input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_DOWN) emiter1->Play(0);
	//if (input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_DOWN) emiter2->Play(0);
	
	// Pauses the physics
	if (input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		pause = !pause;
		npl->PausePhysics(pause);
	}

	// Steps the physics
	npl->Update(dt);

	// Draws the bodies
	for (Body* b : *bodies)
	{
		SDL_Color color = { 0, 0, 0, 50 };

		switch (b->GetClass())
		{
		case BodyClass::STATIC_BODY:  color = { 255,   0,   0, color.a }; break;
		case BodyClass::DYNAMIC_BODY: color = {   0, 255,   0, color.a }; break;
		case BodyClass::LIQUID_BODY:  color = { 100, 100, 255, color.a }; break;
		case BodyClass::GAS_BODY:     color = { 255, 255, 255, (Uint8)(color.a - 40) }; break;
		}

		render->DrawRectangle(b->GetRect(), color);
	}

	// Draws the bodies
	if (collisions != nullptr)
	{
		for (Collision* c : *collisions)
		{
			render->DrawRectangle(c->intersecRect, { 100, 100, 255, 255 });
		}
	}

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::LOGO_SCENE);

	return ret;
}

bool TestOneScene::CleanUp()
{
	RELEASE(npl);

	bodies = nullptr;

	test = nullptr;

	return true;
}
