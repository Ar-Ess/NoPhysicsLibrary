#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"

class D3DelayScene : public Scene
{
public:

	D3DelayScene();

	~D3DelayScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

private:

	NoPhysicsLibrary* physics = nullptr;

};
