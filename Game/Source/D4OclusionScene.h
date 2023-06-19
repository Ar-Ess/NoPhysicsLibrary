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

private:

	NoPhysicsLibrary* physics = nullptr;
	SDL_Texture* background = nullptr;
	SDL_Texture* groundTex = nullptr;
	DynamicBody* player = nullptr;

};