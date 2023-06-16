#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"

class D2SpatialScene : public Scene
{
public:

	D2SpatialScene();

	~D2SpatialScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

private:

	NoPhysicsLibrary* physics = nullptr;

};
