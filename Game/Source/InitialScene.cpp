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

	if (input->GetKey(SDL_SCANCODE_Z) == KeyState::KEY_DOWN) ret = SetScene(Scenes::DEBUG_SCENE);
	if (input->GetKey(SDL_SCANCODE_0) == KeyState::KEY_DOWN) ret = SetScene(Scenes::EDITOR_SCENE);

	if (input->GetKey(SDL_SCANCODE_1) == KeyState::KEY_DOWN) ret = SetScene(Scenes::D1_SCENE);
	if (input->GetKey(SDL_SCANCODE_2) == KeyState::KEY_DOWN) ret = SetScene(Scenes::D2_SCENE);
	if (input->GetKey(SDL_SCANCODE_3) == KeyState::KEY_DOWN) ret = SetScene(Scenes::D3_SCENE);
	if (input->GetKey(SDL_SCANCODE_4) == KeyState::KEY_DOWN) ret = SetScene(Scenes::D4_SCENE);
	if (input->GetKey(SDL_SCANCODE_5) == KeyState::KEY_DOWN) ret = SetScene(Scenes::D5_SCENE);

	return ret;
}

bool InitialScene::CleanUp()
{
	return true;
}
