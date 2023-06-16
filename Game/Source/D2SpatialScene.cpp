#include "D2SpatialScene.h"

D2SpatialScene::D2SpatialScene()
{
}

D2SpatialScene::~D2SpatialScene()
{
}

bool D2SpatialScene::Start()
{
	physics = new NoPhysicsLibrary();
	physics->Init(256);

	return true;
}

bool D2SpatialScene::Update(float dt)
{
	physics->Update(dt);

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

	return true;
}

bool D2SpatialScene::CleanUp()
{
	physics->CleanUp();
	delete physics;
	physics = nullptr;

	return true;
}
