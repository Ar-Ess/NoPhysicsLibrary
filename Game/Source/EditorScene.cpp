#include "EditorScene.h"
#include "Vector.h"

EditorScene::EditorScene()
{
}

EditorScene::~EditorScene()
{
}

bool EditorScene::Start()
{
	Point wSize = window->GetSize();
	npl = new NPL();
	npl->Init(128);
	npl->Configure()->PanRange(10, InUnit::IN_METERS);
	npl->Configure()->PhysicsIterations(40);
	npl->SetPhysicsPreset(PhysicsPreset::DEFAULT_PHYSICS_PRESET);
	npl->Configure()->Listener(player);
	npl->LoadSound("Assets/Audio/bounce.wav");


	player = npl->CreateBody({ 100.0f, 200.0f, 38.0f, 90.0f })
		->Dynamic(80);

	// Ground
	npl->CreateBody(0, wSize.y - 20, 4000, 20)->Static();
	npl->CreateBody(-20, 0, 20, 720)->Static();

	render->ResetCamera();

	return true;
}

bool EditorScene::Update(float dt)
{
	UpdateLevel(dt);
	DrawLevel();

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

	return true;
}

bool EditorScene::CleanUp()
{
	if (!npl)
	{
		npl->CleanUp();
		RELEASE(npl);
	}

	player = nullptr;
	return true;
}

bool EditorScene::UpdateLevel(float dt)
{
	bool ground = player->IsBodyStill(BodyState::ON_GROUND);
	bool shift = (input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT);

	// Camera movement
	if (player->GetPosition(InUnit::IN_PIXELS).x >= 625) render->camera.x = player->GetPosition(InUnit::IN_PIXELS).x - 625;
	if (render->camera.x < 0) render->camera.x = 0;
	if (render->camera.x > 3000) render->camera.x = 3000;
	if (render->camera.y < 0) render->camera.y = 0;

	// Inputs
	if (input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
	{
		player->ApplyMomentum(20, 0);
		if (ground && shift) player->ApplyForce(30, 0);
	}

	if (input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	{
		player->ApplyMomentum(-20, 0);
		if (ground && shift) player->ApplyForce(-30, 0);
	}

	if (ground && input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		float plus = 0;
		if (shift && player->IsBodyStill(BodyState::MOVING))
		{
			plus = -65;
		}
		player->ApplyMomentum(0, -400 + plus);
	}

	npl->Update(&dt);

	return true;
}

bool EditorScene::DrawLevel()
{
	unsigned int size = npl->Get()->BodiesCount();
	for (unsigned int i = 0; i < size; ++i)
	{
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
		render->DrawRectangle(Rect{ b->GetEmissionPoint(InUnit::IN_PIXELS).Apply({-3.0f, -3}), 6, 6 }, { 155, 255, 155, 255 });
	}

	return true;
}
