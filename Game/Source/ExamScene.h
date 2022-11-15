#pragma once

#include "Scene.h"
//-Exercice 1: Set the following variables:
// -> A variable for the library (ptr*), 
// -> A variable for a player (ptr*)
// -> A variable for an inmovile object that will emit sound (ptr*)

class ExamScene : public Scene
{
public:

	ExamScene();

	~ExamScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

private:

	// Write here the variables

};
