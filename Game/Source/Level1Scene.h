#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"

class Level1Scene : public Scene
{
public:

	Level1Scene();

	~Level1Scene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

private:

	bool UpdateLevel(float dt);

	bool DrawLevel();

private:

	NPL* npl = nullptr;
	DynamicBody* player = nullptr;

};