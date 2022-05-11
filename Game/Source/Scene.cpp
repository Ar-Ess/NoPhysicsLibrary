#include "Scene.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(Render* render, Input* input, Textures* texture, Window* window, Audio* audio)
{
	this->gui = new GuiManager(input, render, audio, texture);
	this->render = render;
	this->input = input;
	this->texture = texture;
	this->window = window;
	this->audio = audio;
}

Scene::~Scene()
{}

bool Scene::Start()
{
	//DEBUG BOOLS
	gui->Start(this);

	//FIRST SCENE
	if (!SetScene(LOGO_SCENE)) return false;

	//CONTINUE ACTIVITY
	/*activeContinue = false;
	if (FILE* file = fopen("save_game.xml", "r"))
	{
		fclose(file);
		activeContinue = true;
	}*/

	render->SetScale(1); //Qui toqui aquesta linia de codi, la 72, i m'entero, no viu un dia més :) <3

	return true;
}

bool Scene::Update(float dt)
{
	bool ret = true;

	switch (currScene)
	{
	case LOGO_SCENE:
		ret = UpdateLogoScene(dt);
		break;

	case MAIN_MENU_SCENE:
		ret = UpdateMainMenuScene(dt);
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
	case LOGO_SCENE:
		break;

	case MAIN_MENU_SCENE:
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
	case LOGO_SCENE:
		ret = SetLogoScene();
		break;

	case MAIN_MENU_SCENE:
		ret = SetMainMenuScene();
		break;
	}

	//easing.ResetIterations();

	return ret;
}

bool Scene::SetLogoScene()
{
	return true;
}

bool Scene::SetMainMenuScene()
{
	return true;
}

bool Scene::UpdateLogoScene(float dt)
{
	bool ret = true;

	if (input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) ret = SetScene(Scenes::MAIN_MENU_SCENE);

	return ret;
}

bool Scene::UpdateMainMenuScene(float dt)
{
	bool ret = true;

	return ret;
}

// GUI CONTROLS

bool Scene::OnGuiMouseClickEvent(GuiControl* control, float value, bool check)
{
	switch (currScene)
	{
	case MAIN_MENU_SCENE:
		switch (control->id)
		{
			break;
		}
		break;
	}

	return true;
}

void Scene::DebugCommands()
{
	if (input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) gui->debug = !gui->debug;

	if (input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) window->SetWinFullScreen(!window->fullScreen);

	switch (currScene)
	{
	case LOGO_SCENE:
		break;
	}
}
