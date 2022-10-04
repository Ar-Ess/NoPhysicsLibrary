#include "TestTwoScene.h"

TestTwoScene::TestTwoScene()
{
}

TestTwoScene::~TestTwoScene()
{
	if (npl) npl->CleanUp();
}

bool TestTwoScene::Start()
{
	npl = new NPL();
	npl->Init();
	npl->Configure().PixelsToMeters(6);
	npl->Configure().CollisionsDebugging(false);
	npl->Configure().PanRange(10, InUnit::IN_METERS);

	player = npl->CreateBody(Rect(100, 200, 20, 45), 10).Dynamic();
	emitter = npl->SetScenarioPreset(ScenarioPreset::CORRIDOR_SCENARIO_PRESET, window->GetSize(), 1);
	npl->CreateBody(npl->ReturnScenarioRect(), 1).Gas(10, 1.414f, 1000);

	npl->LoadSound("Assets/Audio/bounce.wav");
	npl->SetListener(player);

	npl->SetGlobalGravity({0, 10});

	return true;
}

bool TestTwoScene::Update(float dt)
{
	if (input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT) player->ApplyMomentum(  20, 0);
	if (input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT) player->ApplyMomentum( -20, 0);
	if (input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN) player->ApplyMomentum(0, -600);

	if (input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_DOWN) player->Play(0);
	if (input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) emitter->Play(0);

	// Update library
	npl->Update(&dt);

	// Draw bodies
	int size = 1;
	for (int i = 0; i < size; ++i)
	{
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

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::LOGO_SCENE);

	return true;
}

bool TestTwoScene::CleanUp()
{
	npl->CleanUp();
	RELEASE(npl);

	player = nullptr;
	emitter = nullptr;

	return true;
}