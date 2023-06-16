#include "D1WorldScene.h"

D1WorldScene::D1WorldScene()
{
}

D1WorldScene::~D1WorldScene()
{
}

bool D1WorldScene::Start()
{
	physics = new NoPhysicsLibrary();
	physics->Init(256);
	return true;
}

bool D1WorldScene::Update(float dt)
{
	physics->Update(dt);

	return true;
}

bool D1WorldScene::CleanUp()
{
	physics->CleanUp();
	delete physics;
	physics = nullptr;

	return true;
}
