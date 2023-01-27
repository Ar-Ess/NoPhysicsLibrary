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
	static int counter = 4;
	static int subCounter = 2;
	static bool change = true;
	static bool active = true;
	bool ret = true;

	rect.SetPosition(ease.Ease({ 100.0f, 360 }, { 1180.0f, 360 }, dt));

	render->DrawRectangle(rect);

	if (input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN)
	{
		if (subCounter == 2)
		{
			subCounter = -1;
			counter++;
		}

		subCounter++;

		ease.OnEaseEnd((EaseType)counter, (EaseMode)subCounter);

		if (counter == 10 && subCounter == 2) counter = 0;
	}

	if (input->GetKey(SDL_SCANCODE_Z) == KeyState::KEY_DOWN) ret = SetScene(Scenes::TEST_SCENE);
	if (input->GetKey(SDL_SCANCODE_X) == KeyState::KEY_DOWN) ret = SetScene(Scenes::EXAM_SCENE);

	if (input->GetKey(SDL_SCANCODE_0) == KeyState::KEY_DOWN) ret = SetScene(Scenes::EDITOR_SCENE);

	return ret;
}

bool InitialScene::CleanUp()
{
	return true;
}
