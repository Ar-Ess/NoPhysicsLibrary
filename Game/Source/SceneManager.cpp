#include "SceneManager.h"
#include "LogoScene.h"
#include "TestOneScene.h"

SceneManager::SceneManager(Render* render, Input* input, Window* window)
{
	this->assets = new AssetsManager();
	this->texture = new Textures(render, assets);
	this->render = render;
	this->input = input;
	this->window = window;
}

SceneManager::~SceneManager()
{}

bool SceneManager::Start()
{
	assets->Start();
	texture->Start();
	render->SetScale(1); //Qui toqui aquesta linia de codi, la 20, i m'entero, no viu un dia m�s :) <3

	PushScene(new LogoScene());
	PushScene(new TestOneScene());

	ChangeScene(0);

	return true;
}

bool SceneManager::Update(float dt)
{
	bool ret = true;

	if (currScene <= -1) return true;

	ret = scenes[currScene]->Update(dt);

	if (changeScene > -1) ChangeScene(changeScene);

	return ret;
}

bool SceneManager::CleanUp()
{
	LOG("Freeing scene manager");

	for (Scene* scene : scenes) RELEASE(scene);

	scenes.clear();
	scenes.shrink_to_fit();

	return true;
}

bool SceneManager::ChangeScene(int scene)
{
	bool ret = true;

	if (currScene > -1) ret = scenes[currScene]->CleanUp();

	prevScene = currScene;
	currScene = scene;

	ret = scenes[currScene]->Start();

	changeScene = -1;

	return ret;
}

bool SceneManager::PushScene(Scene* scene)
{
	bool ret = true;

	scenes.emplace_back(scene);

	scene->SetModules(render, input, texture, assets, window, this);

	return ret;
}