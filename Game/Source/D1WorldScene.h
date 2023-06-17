#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"

class SDL_Texture;

class D1WorldScene : public Scene
{
public:

	D1WorldScene();

	~D1WorldScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void Draw();

private:

	NoPhysicsLibrary* physics = nullptr;
	SDL_Texture* background = nullptr;
	DynamicBody* player = nullptr;

};
