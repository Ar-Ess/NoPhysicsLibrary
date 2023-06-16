#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"

class D5PitchScene : public Scene
{
public:

	D5PitchScene();

	~D5PitchScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

private:

	NoPhysicsLibrary* physics = nullptr;

};
