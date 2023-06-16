#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"

class DebugScene : public Scene
{
public:

	DebugScene();

	~DebugScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

private:

	NPL* npl = nullptr;
	DynamicBody* player = nullptr;
	DynamicBody* shell = nullptr;
	StaticBody* emitter = nullptr;
};

