#pragma once

#include "Input.h"
#include "Window.h"
#include "Textures.h"
#include "AssetsManager.h"

class SceneManager;

enum class Scenes
{
	NO_SCENE = -1,
	INITIAL_SCENE,
	TEST_SCENE,
	EXAM_SCENE,
};

class Scene
{
public: // Methods

	virtual ~Scene() {}

	virtual bool Start() { return true; }

	virtual bool Update(float dt) { return true; }

	virtual bool CleanUp() { return true; }

	void SetModules(Render* render, Input* input, Textures* texture, AssetsManager* assets, Window* window, SceneManager* scene);

protected: // Methods

	Scene() {}

	bool SetScene(Scenes index);

	bool PushScene(Scene* push);

protected: // Variables

	Render* render = nullptr;
	Input* input = nullptr;
	Textures* texture = nullptr;
	AssetsManager* assets = nullptr;
	Window* window = nullptr;

private: // Variables

	SceneManager* scene = nullptr;

};