#pragma once

#include "Module.h"
#include "Scene.h"
#include <vector>

class SceneManager : public Module
{
public:

	SceneManager(Render* render, Input* input, Window* window);

	virtual ~SceneManager();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	bool PushScene(Scene* scene);

	inline size_t GetSize() const { return scenes.size(); }

private:

	bool ChangeScene(int scene);

public:

	int changeScene = -1; // Flag to change scene index

private: // Variables

	int currScene = -1;
	int prevScene = -1;

	Render* render = nullptr;
	Input* input = nullptr;
	Textures* texture = nullptr;
	AssetsManager* assets = nullptr;
	Window* window = nullptr;

	std::vector<Scene*> scenes;

};