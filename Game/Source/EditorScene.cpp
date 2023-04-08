#include "EditorScene.h"

EditorScene::EditorScene()
{
}

EditorScene::~EditorScene()
{
}

bool EditorScene::Start()
{	
	Point wSize = window->GetSize();
	physics = new NPL();
	physics->Init(128);

	player = new Player(physics->CreateBody({ 100.0f, 1120.0f, 38.0f, 90.0f })->Dynamic(80), input);
	map = new TileManager({ 24.0f, 27 }, render, window, physics);

	physics->Configure()->PanRange(10, InUnit::IN_METERS);
	physics->Configure()->PhysicsIterations(40);
	physics->SetPhysicsPreset(PhysicsPreset::DEFAULT_PHYSICS_PRESET);
	physics->Configure()->Listener(player->Body());
	//physics->LoadSound("Assets/Audio/bounce.wav");

	// Ground
	physics->CreateBody(0, wSize.y * 2 - 20, 4000, 20)->Static();
	physics->CreateBody(-20, 0, 20, 720)->Static();

	physics->PausePhysics(editMode);
	render->ResetCamera({0.f, 720});

	return true;
}

bool EditorScene::Update(float dt)
{
	Point pPos = player->Position();

	// Update
	UpdateEditMode(dt);
	UpdatePlayMode(dt);

	// Camera
	render->camera.Update(pPos)->LimitFollow({0, 0, 200, 720});

	// Physics
	physics->Update(&dt);

	// Inputs
	UpdateGeneralInputs();

	return true;
}

bool EditorScene::Draw(float dt)
{
	DrawEditMode(dt);

	DrawPlayMode(dt);

	return true;
}

bool EditorScene::CleanUp()
{
	if (physics)
	{
		physics->CleanUp();
		RELEASE(physics);
	}

	delete player;
	player = nullptr;
	return true;
}

bool EditorScene::UpdateEditMode(float dt)
{
	if (!editMode) return true;

	map->Update(dt);

	if (input->GetKey(SDL_SCANCODE_0) == KeyState::KEY_DOWN) map->SetTile(TileType::NO_TILE);
	else if (input->GetKey(SDL_SCANCODE_1) == KeyState::KEY_DOWN) map->SetTile(TileType::GROUND_TILE);

	if (input->GetMouseButtonDown(1) == KeyState::KEY_DOWN) map->PlaceTile(input->GetMousePosition() + render->camera.rect.GetPosition());

	return true;
}

bool EditorScene::DrawEditMode(float dt)
{
	if (!editMode) return true;

	map->Draw(dt, true, true);

	return true;
}

bool EditorScene::UpdatePlayMode(float dt)
{
	if (editMode) return true;

	map->Update(dt);

	player->Update(dt);

	return true;
}

bool EditorScene::DrawPlayMode(float dt)
{
	if (editMode) return true;

	map->Draw(dt, false, true);

	return true;
}

bool EditorScene::UpdateGeneralInputs()
{
	if (input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
	{
		editMode = !editMode;
		physics->PausePhysics(editMode);
	}

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

	return true;
}

// --------------------------------------------------
