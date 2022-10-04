#pragma once

#include "Auxiliars.h"
// Includes: Body.h, Audio.h 

#include "ScenarioPresetEnum.h"
#include "PhysicsPresetEnum.h"
#include "SoundData.h"

// Temp
#include "Collision.h"

class NPL
{
public:

	NPL();

	~NPL();

	// Initialize the library
	void Init();

	// Clean Up and reset the library
	void CleanUp();

	// Create a new body
	//  - Follow this function with a "." to choose the body type
	BodyCreation CreateBody(Rect rectangle, float mass);

	// Configure certain aspects of this library operation
	LibraryConfig Configure();

	// Iterates the library
	//  - Call it in your main update function
	void Update(float* dt);

	// Set a predesigned scenario
	//  - This function destroys automatically the previous scenario
	StaticBody* SetScenarioPreset(ScenarioPreset preset, Point windowSize = { 1080, 720 }, int returnStatic = -1);

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

	// Load internally an audio file
	void LoadSound(const char* path);

	const Collision* GetCollisionsIterable(int &size, int index);

	const Body* GetBodiesIterable(int& size, int index);

private: // Methods

	void StepPhysics(float dt);

	void StepAcoustics();
		void ListenerLogic(Body* b, GasBody* environment);
			float ComputePanning(float distance, float bodyX);
			float ComputeVolume(float distance, float spl);
			float ComputeTimeDelay(float distance, GasBody* environment);
		void NoListenerLogic(Body* b);

	void StepAudio(float* dt);

	bool EraseBody(Body* body);

	inline bool IsVoid() const { return gasIndex.empty(); }

	GasBody* GetEnvironmentBody(Rect body);

private:

	Physics* physics = nullptr;
	Audio* audio = nullptr;
	Body* listener = nullptr;

	std::vector<Body*> bodies;
	std::vector<SoundData*> soundDataList;
	// Fet aixi perquè amb unic ids s'hauria de fer moltes iteracions + protegir el vector de bodies
	std::vector<unsigned int*> gasIndex;

	// CONSTANTS
	const float maxSPL = 120.0f;
	const float maxVolume = 10.0f;

	//_____________________
	// CONFIG VARIABLES
	// - Physics

	// Debug Collisions (0) |
	Flag physicsConfig = {};

	//_____________________
	// - Acoustics
	float panRange = 10.0f;
	float pixelsToMeters = 20.0f;

	//_____________________
	// - Audio

};