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

	if (input->GetKey(SDL_SCANCODE_1) == KeyState::KEY_DOWN) ret = SetScene(Scenes::TEST_SCENE);

	return ret;
}

bool InitialScene::CleanUp()
{
	return true;
}
