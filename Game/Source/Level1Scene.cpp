#include "Level1Scene.h"
#include "Vector.h"

Level1Scene::Level1Scene()
{
}

Level1Scene::~Level1Scene()
{
}

bool Level1Scene::Start()
{
	npl = new NPL();
	npl->Init(128);

	render->ResetCamera();

	return true;
}

bool Level1Scene::Update(float dt)
{

	//Change scene
	if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KeyState::KEY_DOWN) SetScene(Scenes::INITIAL_SCENE);

	return true;
}

bool Level1Scene::CleanUp()
{
	return true;
}