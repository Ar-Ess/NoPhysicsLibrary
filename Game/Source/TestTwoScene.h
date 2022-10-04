#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"

class TestTwoScene : public Scene
{
public:
	
	TestTwoScene();

	~TestTwoScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

private:

	NPL* npl = nullptr;
	DynamicBody* player = nullptr;
	StaticBody* emitter = nullptr;

};