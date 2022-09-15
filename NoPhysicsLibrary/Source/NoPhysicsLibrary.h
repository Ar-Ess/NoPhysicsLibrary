#pragma once

#include "BodyCreationAux.h" 
// Includes: Body.h, Audio.h 

#include "ScenarioPresetEnum.h"
#include "PhysicsPresetEnum.h"
#include "SoundData.h"

class NPL
{
public:

	std::vector<Body*>* TempGetBodiesDebug()
	{
		return &bodies;
	}

	NPL();

	~NPL();

	// Initialize the library
	void Init();

	// Clean Up and reset the library
	void CleanUp();

	// Create a new body
	//  - Follow this function with a "." to choose the body type
	BodyCreation CreateBody(Rect rectangle, float mass);

	// Iterates the library
	//  - Call it in your main update function
	void Update(float dt);

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

	// Returns the global gravity vector
	Point GetGlobalGravity() const;
	// Sets if global gravity is active and its vector
	void SetGlobalGravity(Point vector);

	// Returns the global friction vector
	Point GetGlobalFriction() const;
	// Sets if global friction is active and its vector
	void SetGlobalFriction(Point vector);

	// Returns the global restitution vector
	Point GetGlobalRestitution() const;
	// Sets if global restitution is active and its vector
	void SetGlobalRestitution(Point vector);

	// Destroys a body whenever it's outside the rectangle setted. 
	//    Returns true when a body is destroyed
	bool DeathLimit(Rect limits);

	// Destroy a body, returns true if the body has been successfully deleted
	bool DestroyBody(Body* body);
	
	// Sets one body to become the listening point of the audio
	void SetListener(Body* listener);

	void LoadSound(const char* path);

private: // Methods

	void StepPhysics(float dt);

	void StepAcoustics();

	void StepAudio();

	bool EraseBody(Body* body);

private:

	Physics* physics = nullptr;
	Audio* audio = nullptr;
	Body* listener = nullptr;

	// -TODONE: Investigate about vector. Maybe not the best option if users has to save a pointer to data
	// inside here. Vectors reorder themselves if they don't find enough space. And Idk if that could
	// lead to a "nullptr" error. Look video: https://www.youtube.com/watch?v=6OoSgY6NVVk&t=1550s&ab_channel=javidx9
	std::vector<Body*> bodies;
	std::vector<SoundData*> soundList;

	// CONFIG VARIABLES
	float panRadius = 1000.0f;

};