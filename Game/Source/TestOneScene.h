#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"
#include "Body.h"
#include "Collision.h"
#include "DynamicBody.h"
#include <vector>

class TestOneScene : public Scene
{
public:
	
	TestOneScene();

	~TestOneScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

private:

	NPL* npl = nullptr;
	DynamicBody* test = nullptr;
	StaticBody* emitter = nullptr;
};