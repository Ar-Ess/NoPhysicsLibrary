#include "Scene.h"
#include "SceneManager.h"

void Scene::SetModules(Render* render, Input* input, Textures* texture, AssetsManager* assets, Window* window, SceneManager* scene)
{
	this->render = render;
	this->input = input;
	this->texture = texture;
	this->assets = assets;
	this->window = window;
	this->scene = scene;
}

bool Scene::SetScene(Scenes index)
{
	bool ret = true;

	unsigned int sceneIndex = (unsigned int)index;

	if (sceneIndex < 0 || sceneIndex >= scene->GetSize()) return false;

	scene->changeScene = sceneIndex;

	return ret;
}

bool Scene::PushScene(Scene* push)
{
	scene->PushScene(push);

	return true;
}

Scenes Scene::GetCurrentScene() const
{
	return (Scenes)scene->currScene;
}

Scenes Scene::GetPreviousScene() const
{
	return (Scenes)scene->prevScene;
}
