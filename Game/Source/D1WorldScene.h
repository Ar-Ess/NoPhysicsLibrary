#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"

class D1WorldScene : public Scene
{
public:

	D1WorldScene();

	~D1WorldScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

private:

	NoPhysicsLibrary* physics = nullptr;

};
