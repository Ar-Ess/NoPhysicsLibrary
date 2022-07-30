#include "Scene.h"
#include "NoPhysicsLibrary.h"

Scene::Scene(Render* render, Input* input, Window* window)
{
	this->assets = new AssetsManager();
	this->texture = new Textures(render, assets);
	this->audio = new Audio(assets);
	this->render = render;
	this->input = input;
	this->window = window;
}

Scene::~Scene()
{}

bool Scene::Start()
{
	assets->Start();
	texture->Start();
	audio->Start();
	NPL* npl = new NPL();
	npl->Init();


	//FIRST SCENE
	if (!SetScene(Scenes::LOGO_SCENE)) return false;

	render->SetScale(1); //Qui toqui aquesta linia de codi, la 72, i m'entero, no viu un dia més :) <3

	return true;
}

bool Scene::Update(float dt)
{
	bool ret = true;

	switch (currScene)
	{
	case Scenes::LOGO_SCENE:
		ret = UpdateLogoScene(dt);
		break;

	case Scenes::DEBUG_SCENE:
		ret = UpdateDebugScene(dt);
		break;
	}

	DebugCommands();

	return (!exit && ret);
}

bool Scene::CleanUp()
{
	LOG("Freeing scene");

	switch (currScene)
	{
	case Scenes::LOGO_SCENE:
		break;

	case Scenes::DEBUG_SCENE:
		break;
	}

	return true;
}

// SCENE MANAGER

bool Scene::SetScene(Scenes scene)
{
	bool ret = true;
	CleanUp();

	prevScene = currScene;
	currScene = scene;

	switch (currScene)
	{
	case Scenes::LOGO_SCENE:
		ret = SetLogoScene();
		break;

	case Scenes::DEBUG_SCENE:
		ret = SetDebugScene();
		break;
	}

	//easing.ResetIterations();

	return ret;
}

bool Scene::SetLogoScene()
{
	return true;
}

bool Scene::SetDebugScene()
{
	return true;
}

bool Scene::UpdateLogoScene(float dt)
{
	bool ret = true;

	if (input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN) ret = SetScene(Scenes::DEBUG_SCENE);

	return ret;
}

bool Scene::UpdateDebugScene(float dt)
{
	bool ret = true;

	return ret;
}

void Scene::DebugCommands()
{
	if (input->GetKey(SDL_SCANCODE_F11) == KeyState::KEY_DOWN) window->SetWinFullScreen(!window->fullScreen);

	switch (currScene)
	{
	case Scenes::LOGO_SCENE:
		break;
	}
}
