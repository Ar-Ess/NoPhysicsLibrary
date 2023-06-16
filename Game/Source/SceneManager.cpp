#include "SceneManager.h"
#include "InitialScene.h"
#include "DebugScene.h"
#include "EditorScene.h"
#include "D1WorldScene.h"
#include "D2SpatialScene.h"
#include "D3DelayScene.h"
#include "D4OclusionScene.h"
#include "D5PitchScene.h"

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
	PushScene(new DebugScene());
	PushScene(new EditorScene());
	PushScene(new D1WorldScene());
	PushScene(new D2SpatialScene());
	PushScene(new D3DelayScene());
	PushScene(new D4OclusionScene());
	PushScene(new D5PitchScene());

	ChangeScene(0);

	return true;
}

bool SceneManager::Update(float dt)
{
	bool ret = true;

	if (currScene <= -1) return true;

	ret = scenes[currScene]->Update(dt);

	ret = scenes[currScene]->Draw(dt);

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
