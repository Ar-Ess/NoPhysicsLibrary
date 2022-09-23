#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"
#include "Body.h"
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
	const std::vector<Body*>* bodies = nullptr;
	DynamicBody* test = nullptr;
	StaticBody* emiter1 = nullptr;
	StaticBody* emiter2 = nullptr;
};