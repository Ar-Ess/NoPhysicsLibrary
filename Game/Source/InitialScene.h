#pragma once

#include "Scene.h"

class InitialScene : public Scene
{
public:
	
	InitialScene();

	~InitialScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

};