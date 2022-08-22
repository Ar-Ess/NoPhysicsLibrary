#pragma once

#include "Module.h"

#include "Input.h"
#include "Window.h"
#include "Audio.h"
#include "Textures.h"
#include "AssetsManager.h"
#include "NoPhysicsLibrary.h"

enum class Scenes
{
	NO_SCENE = -1,
	LOGO_SCENE,
	DEBUG_SCENE,
};

class Scene : public Module
{
public:

	Scene(Render* render, Input* input, Window* window);

	virtual ~Scene();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

// SCENE MANAGER

	inline Scenes GetCurrScene() const { return currScene; }

	bool SetScene(Scenes scene);

private:
	Scenes currScene = Scenes::NO_SCENE;
	Scenes prevScene = Scenes::NO_SCENE;

	//Setters
	bool SetLogoScene();
	bool SetDebugScene();

	//Updaters
	bool UpdateLogoScene(float dt);
	bool UpdateDebugScene(float dt);

private: // Methods

	void DebugCommands();

private: // Variables

	Render* render = nullptr;
	Input* input = nullptr;
	Textures* texture = nullptr;
	AssetsManager* assets = nullptr;
	Window* window = nullptr;
	Audio* audio = nullptr;

	NPL* physics = nullptr;
	std::vector<Body*> bodies;

	bool exit = false;
	bool activeContinue = false;

	suint lvl = 0;
};