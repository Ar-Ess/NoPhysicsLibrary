#include "D3DelayScene.h"

D3DelayScene::D3DelayScene()
{
}

D3DelayScene::~D3DelayScene()
{
}

bool D3DelayScene::Start()
{
	physics = new NoPhysicsLibrary();
	physics->Init(256);

	return true;
}

bool D3DelayScene::Update(float dt)
{
	physics->Update(dt);

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

	return true;
}

bool D3DelayScene::CleanUp()
{
	physics->CleanUp();
	delete physics;
	physics = nullptr;

	return true;
}
