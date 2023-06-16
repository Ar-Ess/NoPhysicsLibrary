#include "D5PitchScene.h"

D5PitchScene::D5PitchScene()
{
}

D5PitchScene::~D5PitchScene()
{
}

bool D5PitchScene::Start()
{
	physics = new NoPhysicsLibrary();
	physics->Init(256);

	return true;
}

bool D5PitchScene::Update(float dt)
{
	physics->Update(dt);

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

	return true;
}

bool D5PitchScene::CleanUp()
{
	physics->CleanUp();
	delete physics;
	physics = nullptr;

	return true;
}
