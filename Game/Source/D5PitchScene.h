#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"

class SDL_Texture;

class D5PitchScene : public Scene
{
public:

	D5PitchScene();

	~D5PitchScene() override;

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
