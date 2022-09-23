#pragma once

#include "Auxiliars.h"
// Includes: Body.h, Audio.h 

#include "ScenarioPresetEnum.h"
#include "PhysicsPresetEnum.h"
#include "SoundData.h"

class NPL
{
public:

	const std::vector<Body*>* TempGetBodiesDebug()
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

	LibraryConfig Configure();

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
	bool DeathLimit(Rect limits, DynamicBody* body);

	// Destroy a body, returns true if the body has been successfully deleted
	bool DestroyBody(Body* body);
	
	// Sets one body to become the listening point of the audio
	void SetListener(Body* listener);

	void LoadSound(const char* path);

private: // Methods

	void StepPhysics(float dt);

	void StepAcoustics();
	void NoListenerLogic(Body* b);
	void ListenerLogic(Body* b, GasBody* environment);
	GasBody* GetEnvironmentBody(Rect body);
	float ComputePanning(float distance, float bodyX);
	float ComputeVolume(float distance, float spl);
	float ComputeTimeDelay(float distance, GasBody* environment);

	void StepAudio();

	bool EraseBody(Body* body);

	inline bool IsVoid() const { return gasLocations.empty(); }

private:

	Physics* physics = nullptr;
	Audio* audio = nullptr;
	Body* listener = nullptr;

	std::vector<Body*> bodies;
	std::vector<SoundData*> soundDataList;
	//-Todone: Llista de index a localitzacio de gasbody
	std::vector<unsigned int*> gasLocations;

	// CONFIG VARIABLES
	float panRange = 1000.0f;

	// CONSTANTS
	const float maxSPL = 120.0f;
	const float maxVolume = 10.0f;

};