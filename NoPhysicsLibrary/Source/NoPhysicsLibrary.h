#pragma once
#include "BodyCreationAux.h"
#include "Body.h"
#include "ScenarioPresetEnum.h"
#include "PhysicsPresetEnum.h"
#include "Audio.h"

class NPL
{
public:

	NPL();

	// Initialize the library
	void Init();

	// Clean Up and reset the library
	void CleanUp();

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
	// Allows to pause or unpause the physics
	void PausePhysics(bool pause);

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

	// Destroys a body whenever it's outside the rectangle setted. 
	//    Returns true when a body is destroyed
	bool DeathLimit(Rect limits);

	// Destroy a body, returns true if the body has been successfully deleted
	bool DestroyBody(Body* body);

	// -TODO: Function to set an specific body as listener to calculate pan and stuff

	void LoadSound(const char* path);

	void ShowcaseLoadAudio(const char* path)
	{
		audio->TestLoadAudio(path);
	}

	void ShowcasePlayAudio(float distance = 0, bool shiftVolume = false, bool shiftBoth = false)
	{
		audio->TestPlayAudio(distance, shiftVolume, shiftBoth);
	}

	void ShowcaseUnloadAudio()
	{
		audio->TestUnloadAudio();
	}

private:

	Physics* physics = nullptr;
	Audio* audio = nullptr;

};