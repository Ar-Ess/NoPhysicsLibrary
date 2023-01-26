#pragma once

#include "Scene.h"

#include "Easing.h"

class InitialScene : public Scene
{
public:
	
	InitialScene();

	~InitialScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	Easing ease = Easing(EaseType::LINEAR, EaseMode::IN_MODE);
	Rect rect = Rect(100, 360, 10, 10);

};