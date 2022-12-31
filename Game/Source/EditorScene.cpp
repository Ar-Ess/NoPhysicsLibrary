#include "EditorScene.h"
#include "PerfTimer.h"

EditorScene::EditorScene()
{
}

EditorScene::~EditorScene()
{
}

bool EditorScene::Start()
{
	// Iteration testing
	/*PerfTimer timer1;
	//int result1[5] = {};
	//for (int i = 0; i < gred.Size(); ++i)
	//{
	//	result1[i] = *gred[i];
	//}
	//double a = timer1.ReadMs();

	//PerfTimer timer2;
	//int result2[5] = {};
	//uint ret = 0;
	//for (int j = 0; j < gred.Heigth(); ++j)
	//{
	//	for (int i = 0; i < gred.Width(); ++i)
	//	{
	//		if (!gred.Empty(i, j))
	//		{
	//			result2[ret] = *gred.At(i, j);
	//			ret++;
	//		}
	//	}
	//}
	//double b = timer2.ReadMs();*/
	// -----------------

	Point wSize = window->GetSize();
	physics = new NPL();
	physics->Init(128);
	physics->Configure()->PanRange(10, InUnit::IN_METERS);
	physics->Configure()->PhysicsIterations(40);
	physics->SetPhysicsPreset(PhysicsPreset::DEFAULT_PHYSICS_PRESET);
	physics->Configure()->Listener(player);
	physics->LoadSound("Assets/Audio/bounce.wav");


	player = physics->CreateBody({ 100.0f, 200.0f, 38.0f, 90.0f })
		->Dynamic(80);

	// Ground
	physics->CreateBody(0, wSize.y * 2 - 20, 4000, 20)->Static();
	physics->CreateBody(-20, 0, 20, 720)->Static();

	physics->PausePhysics(editMode);
	render->ResetCamera();

	return true;
}

bool EditorScene::Update(float dt)
{
	Point pPos = player->GetPosition(InUnit::IN_PIXELS);

	if (editMode) UpdateEditMode(dt);
	else UpdatePlayMode(dt);

	render->camera.Update(pPos)->LimitFollow({0, 0, 200, 720});
	physics->Update(&dt);
	
	if (editMode) DrawEditMode(dt);
	else DrawPlayMode(dt);

	if (input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
	{
		editMode = !editMode;
		physics->PausePhysics(editMode);
	}

	return true;
}

bool EditorScene::Draw(float dt)
{
	unsigned int size = physics->Get()->BodiesCount();
	for (unsigned int i = 0; i < size; ++i)
	{
		const Body* b = physics->Get()->Bodies(i);
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

bool EditorScene::CleanUp()
{
	if (!physics)
	{
		physics->CleanUp();
		RELEASE(physics);
	}

	player = nullptr;
	return true;
}

bool EditorScene::UpdateEditMode(float dt)
{
	for (uint i = 0; i < grid.Size(); ++i) grid[i]->Update();
	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

	return true;
}

bool EditorScene::DrawEditMode(float dt)
{
	for (uint i = 0; i < grid.Size(); ++i) grid[i]->Draw();

	render->DrawGrid({ 0, 0, window->GetSize().Multiply(1.0f, 2) }, { (float)grid.Width(), (float)grid.Heigth() });

	return true;
}

bool EditorScene::UpdatePlayMode(float dt)
{
	bool ground = player->IsBodyStill(BodyState::ON_GROUND);
	bool shift = (input->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT);

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

	return true;
}

bool EditorScene::DrawPlayMode(float dt)
{

	return true;
}

// --------------------------------------------------
