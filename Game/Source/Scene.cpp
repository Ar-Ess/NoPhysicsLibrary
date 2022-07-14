#include "Scene.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(Render* render, Input* input, Window* window)
{
	this->assets = new AssetsManager();
	this->texture = new Textures(render, assets);
	this->audio = new Audio(assets);
	this->render = render;
	this->input = input;
	this->window = window;

	this->physics = new Physics(render);
}

Scene::~Scene()
{}

bool Scene::Start()
{
	assets->Start();
	texture->Start();
	audio->Start();
	physics->Start();

	//FIRST SCENE
	if (!SetScene(Scenes::LOGO_SCENE)) return false;

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
		physics->CleanUp();
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
	physics->SetScenarioPreset(ScenarioPreset::WALLJUMP_1280x720_SCENARIO_PRESET);
	physics->SetPhysicsPreset(PhysicsPreset::PLATFORMER_PHYSICS_PRESET);
	physics->SetGlobalFriction({0.5f, 0.0f});
	player = (DynamicBody*)physics->CreateBody(BodyType::DYNAMIC_BODY, { 120, 120 }, {120, 120, 50, 20});
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

	physics->Update(dt);

	if (input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT) player->Move(5, Direction::LEFT);
	if (input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT) player->Move(5, Direction::RIGHT);
	if (input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN) player->Jump(300.0f, true);
	if (input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN) player->WallJump({250.0f, 200.0f});

	physics->Draw(dt, player);

	//if (physics->debugBools) gui->Draw();

	return ret;
}

void Scene::DebugCommands()
{
	if (input->GetKey(SDL_SCANCODE_F1) == KeyState::KEY_DOWN)
	{
		//gui->debug = !gui->debug;
		physics->debug = !physics->debug;
	}

	if (input->GetKey(SDL_SCANCODE_F2) == KeyState::KEY_DOWN)
	{
		physics->debugBools = !physics->debugBools;
	}

	if (input->GetKey(SDL_SCANCODE_F11) == KeyState::KEY_DOWN) window->SetWinFullScreen(!window->fullScreen);

	switch (currScene)
	{
	case Scenes::LOGO_SCENE:
		break;
	}
}
