#include "SceneManager.h"
#include "InitialScene.h"
#include "TestThreeScene.h"
#include "Level1Scene.h"
#include "ExamScene.h"

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

	PushScene(new InitialScene());
	PushScene(new TestThreeScene());
	PushScene(new ExamScene());
	PushScene(new Level1Scene());

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

	scenes[currScene]->CleanUp();

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
