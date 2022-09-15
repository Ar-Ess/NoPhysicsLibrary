#include "LogoScene.h"

LogoScene::LogoScene()
{
}

LogoScene::~LogoScene()
{
}

bool LogoScene::Start()
{
	return true;
}

bool LogoScene::Update(float dt)
{
	bool ret = true;

	if (input->GetKey(SDL_SCANCODE_1) == KeyState::KEY_DOWN) ret = SetScene(Scenes::TEST_ONE_SCENE);

	return ret;
}

bool LogoScene::CleanUp()
{
	return true;
}
