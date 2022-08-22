#pragma once
#include "BodyCreationAux.h"
#include "Body.h"
#include "ScenarioPresetEnum.h"
#include "PhysicsPresetEnum.h"

class NPL
{
public:

	NPL();

	// Initialize the library
	void Init();

	// Create a new body
	//  - Follow this function with a "." to choose the body type
	BodyCreation CreateBody(Rect rectangle, float mass);

	// Iterates the physics
	//  - Call it in your main update function
	void Step(float dt);

	// Set a predesigned scenario
	//  - This function destroys automatically the previous scenario
	void SetScenarioPreset(ScenarioPreset preset, Point windowSize = { 1080, 720 });

	// Destroys all Static Bodies created
	void DestroyScenario();

	// Calculates & returns a rectsngle that englobes all the existent bodies
	Rect ReturnScenarioRect();

	// Returns wether the physics are paused
	bool GetGlobalPause() const;
	// Sets wether the physics are paused or not
	void SetGlobalPause(bool set);

	// Returns the global gravity magnitude
	Point GetGlobalGravity() const;
	// Sets if global gravity is active and its magnitude
	void SetGlobalGravity(Point magnitude);

	// Returns the global friction magnitude
	Point GetGlobalFriction() const;
	// Sets if global friction is active and its magnitude
	void SetGlobalFriction(Point magnitude);

	// Returns the global restitution magnitude
	Point GetGlobalRestitution() const;
	// Sets if global restitution is active and its magnitude
	void SetGlobalRestitution(Point magnitude);

private:

	Physics* physics = nullptr;
};