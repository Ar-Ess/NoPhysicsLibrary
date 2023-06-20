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

private:

	NoPhysicsLibrary* physics = nullptr;

	SDL_Texture* background = nullptr;
	SDL_Texture* groundTex = nullptr;
	SDL_Texture* woodBoxTex = nullptr;
	SDL_Texture* doorTex = nullptr;
	SDL_Texture* rockTex = nullptr;

	DynamicBody* player = nullptr;
	StaticBody* door = nullptr;

};
