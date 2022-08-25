#include "Scene.h"

#define SHOWCASE_MINIAUDIO_ENGINE true

Scene::Scene(Render* render, Input* input, Window* window)
{
	this->assets = new AssetsManager();
	this->texture = new Textures(render, assets);
	this->render = render;
	this->input = input;
	this->window = window;
	this->npl = new NPL();
}

Scene::~Scene()
{}

bool Scene::Start()
{
	assets->Start();
	texture->Start();
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
		npl->CleanUp();
		npl->ShowcaseUnloadAudio(SHOWCASE_MINIAUDIO_ENGINE);
		bodies.clear();
		bodies.shrink_to_fit();
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
	bodies.push_back(npl->CreateBody(Rect{ 100, 167, 200, 67 }, 1).Static());
	bodies.push_back(npl->CreateBody(Rect{ 1100, 120, 60, 98 }, 1).Dynamic());
	test = (DynamicBody*)bodies.back();
	bodies.push_back(npl->CreateBody(Rect{ 1000, 600, 20, 100 }, 1).Liquid());
	bodies.push_back(npl->CreateBody(npl->ReturnScenarioRect(), 1).Gas());
	//npl->SetGlobalGravity({0, 8000});

	npl->ShowcaseLoadAudio("Assets/Audio/bounce.wav", SHOWCASE_MINIAUDIO_ENGINE);

	return true;
}

bool Scene::UpdateLogoScene(float dt)
{
	bool ret = true;

	if (input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) ret = SetScene(Scenes::DEBUG_SCENE);

	return ret;
}

bool Scene::UpdateDebugScene(float dt)
{
	bool ret = true;
	static bool pause = false;

	if (input->GetKey(SDL_SCANCODE_LEFT) == KeyState::KEY_REPEAT) test->ApplyForce(-60000, 0);
	if (input->GetKey(SDL_SCANCODE_RIGHT) == KeyState::KEY_REPEAT) test->ApplyForce(60000, 0);

	if (input->GetKey(SDL_SCANCODE_1) == KeyState::KEY_DOWN) npl->ShowcasePlayAudio(SHOWCASE_MINIAUDIO_ENGINE, 0);
	// -TOCHECK: When DeathLimit() is active, and you bring "test" to a limit, test should point to an empty place, but it doesn't because I can hear the sound still
	if (input->GetKey(SDL_SCANCODE_2) == KeyState::KEY_DOWN) npl->ShowcasePlayAudio(true, test->GetPosition().Apply({-100, 0}).x, false);
	if (input->GetKey(SDL_SCANCODE_3) == KeyState::KEY_DOWN) npl->ShowcasePlayAudio(true, test->GetPosition().Apply({ -100, 0 }).x, true);
	if (input->GetKey(SDL_SCANCODE_4) == KeyState::KEY_DOWN) npl->ShowcasePlayAudio(true, test->GetPosition().Apply({ -100, 0 }).x, false, true);
	
	// Steps the physics
	npl->Step(dt);

	// Draws the bodies
	for (Body* b : bodies)
	{
		SDL_Color color = {0, 0, 0, 50};

		switch (b->GetClass())
		{
		case BodyClass::STATIC_BODY:  color = { 255,   0,   0, color.a }; break;
		case BodyClass::DYNAMIC_BODY: color = {   0, 255,   0, color.a }; break;
		case BodyClass::LIQUID_BODY:  color = { 100, 100, 255, color.a }; break;
		case BodyClass::GAS_BODY:     color = { 255, 255, 255, color.a }; break;
		}

		render->DrawRectangle(b->GetRect(), color);
	}

	// Pauses the physics
	if (input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{
		pause = !pause;
		npl->PausePhysics(pause);
	}

	// Sets death limits
	//if (npl->DeathLimit(render->camera)) LOG("Destroyed");

	if (input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN) SetScene(Scenes::LOGO_SCENE);

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
