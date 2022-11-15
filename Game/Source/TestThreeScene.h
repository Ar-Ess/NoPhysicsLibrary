#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"

class TestThreeScene : public Scene
{
public:

	TestThreeScene();

	~TestThreeScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

private:

	NPL* npl = nullptr;
	DynamicBody* player = nullptr;
	StaticBody* emitter = nullptr;
	StaticBody* gravable = nullptr;
};

