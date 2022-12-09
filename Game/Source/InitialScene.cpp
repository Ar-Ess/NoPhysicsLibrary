#include "InitialScene.h"

InitialScene::InitialScene()
{
}

InitialScene::~InitialScene()
{
}

bool InitialScene::Start()
{
	return true;
}

bool InitialScene::Update(float dt)
{
	bool ret = true;

	if (input->GetKey(SDL_SCANCODE_Z) == KeyState::KEY_DOWN) ret = SetScene(Scenes::TEST_SCENE);
	if (input->GetKey(SDL_SCANCODE_X) == KeyState::KEY_DOWN) ret = SetScene(Scenes::EXAM_SCENE);

	if (input->GetKey(SDL_SCANCODE_1) == KeyState::KEY_DOWN) ret = SetScene(Scenes::LEVEL_1_SCENE);

	return ret;
}

bool InitialScene::CleanUp()
{
	return true;
}
