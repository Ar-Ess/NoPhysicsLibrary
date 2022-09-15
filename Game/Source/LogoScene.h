#pragma once

#include "Scene.h"

class LogoScene : public Scene
{
public:
	
	LogoScene();

	~LogoScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

};