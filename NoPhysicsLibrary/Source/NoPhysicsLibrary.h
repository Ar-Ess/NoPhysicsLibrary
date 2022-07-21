#pragma once
#include "BodyCreationAux.h"
#include "Body.h"
#include "ScenarioPresetEnum.h"
#include "PhysicsPresetEnum.h"

namespace NPL
{
	// Initialize the library
	void Init();

	// Create a new body
	//  - Follow this function with a "." to choose the body type
	BodyCreation CreateBody(Rect rectangle, float mass);

	// Iterates the physics
	//  - Call it in your main update function
	bool Step(float dt);
	
	// Set a predesigned scenario
	//  - This function destroys automatically the previous scenario
	void SetScenarioPreset(ScenarioPreset preset, Point windowSize = {1080, 720});

	// Destroys all Static Bodies created
	void DestroyScenario();

}