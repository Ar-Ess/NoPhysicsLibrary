#include "D4OclusionScene.h"

D4OclusionScene::D4OclusionScene()
{
}

D4OclusionScene::~D4OclusionScene()
{
}

bool D4OclusionScene::Start()
{
	physics = new NoPhysicsLibrary();
	physics->Init(256);

	return true;
}

bool D4OclusionScene::Update(float dt)
{
	physics->Update(dt);

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

	return true;
}

bool D4OclusionScene::CleanUp()
{
	physics->CleanUp();
	delete physics;
	physics = nullptr;

	return true;
}
