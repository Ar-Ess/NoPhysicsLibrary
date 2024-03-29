#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"

class SDL_Texture;

class D2SpatialScene : public Scene
{
public:

	D2SpatialScene();

	~D2SpatialScene() override;

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
	SDL_Texture* airTex = nullptr;
	SDL_Texture* postTex = nullptr;

	DynamicBody* player = nullptr;
	DynamicBody* emmiter = nullptr;
	StaticBody* door = nullptr;

};
