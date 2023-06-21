#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"

class SDL_Texture;

class D4OclusionScene : public Scene
{
public:

	D4OclusionScene();

	~D4OclusionScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void Draw();

	void OclusionLogic();

private:

	NoPhysicsLibrary* physics = nullptr;

	SDL_Texture* background = nullptr;
	SDL_Texture* groundTex = nullptr;
	SDL_Texture* woodBoxTex = nullptr;
	SDL_Texture* doorTex = nullptr;
	SDL_Texture* airTex = nullptr;
	SDL_Texture* postTex = nullptr;
	SDL_Texture* buttonTex = nullptr;
	SDL_Texture* waterTex = nullptr;
	SDL_Texture* clickTex = nullptr;

	DynamicBody* player = nullptr;
	StaticBody* emmiter = nullptr;
	StaticBody* counter = nullptr;
	StaticBody* door = nullptr;
	StaticBody* blockWall = nullptr;
	StaticBody* doorSoundEffectBody = nullptr;
	GasBody* gas = nullptr;
	Body* button = nullptr;
	Body* obstacle = nullptr;

	int step = 0;
	bool buttonClick = false;
	bool buttonTexShow = false;
	Material mat;

};
