#include "EditorScene.h"

EditorScene::EditorScene()
{
}

EditorScene::~EditorScene()
{
}

bool EditorScene::Start()
{
	grid.Set(int(1), 1, 2);
	int i = grid.At(1, 2);
	i = grid.Width();
	i = grid.Heigth();
	i = grid.SizeRow(2);
	i = grid.SizeCol(1);
	i = grid.Size();
	i = grid.SizeMax();

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
	physics->CreateBody(0, wSize.y - 20, 4000, 20)->Static();
	physics->CreateBody(-20, 0, 20, 720)->Static();

	physics->PausePhysics(editMode);
	render->ResetCamera();

	return true;
}

bool EditorScene::Update(float dt)
{
	UpdateEditMode(dt);
	DrawEditMode(dt);

	physics->Update(&dt);

	if (input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
	{
		editMode = !editMode;
		physics->PausePhysics(editMode);
	}

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

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
	if (!editMode) return true;

	return true;
}

bool EditorScene::DrawEditMode(float dt)
{
	if (!editMode) return true;

	return true;
}

bool EditorScene::UpdatePlayMode(float dt)
{
	if (editMode) return true;

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

	return true;
}

bool EditorScene::DrawPlayMode(float dt)
{
	if (editMode) return true;

	return true;
}
