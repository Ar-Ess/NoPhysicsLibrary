#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"

class D4OclusionScene : public Scene
{
public:

	D4OclusionScene();

	~D4OclusionScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

private:

	NoPhysicsLibrary* physics = nullptr;

};
