#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"

class SDL_Texture;

class D3DelayScene : public Scene
{
public:

	D3DelayScene();

	~D3DelayScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void Draw();

	void ChangeMaterial();

private:

	NoPhysicsLibrary* physics = nullptr;

	SDL_Texture* background = nullptr;
	SDL_Texture* groundTex = nullptr;
	SDL_Texture* woodBoxTex = nullptr;
	SDL_Texture* doorTex = nullptr;
	SDL_Texture* airTex = nullptr;
	SDL_Texture* postTex = nullptr;
	SDL_Texture* buttonTex = nullptr;

	DynamicBody* player = nullptr;
	StaticBody* emmiter = nullptr;
	StaticBody* counter = nullptr;
	StaticBody* door = nullptr;
	GasBody* gas = nullptr;
	Body* buttons[2] = {nullptr, nullptr};
	Body* obstacle = nullptr;

	int step = 0;
	bool stepDone = false;

};
