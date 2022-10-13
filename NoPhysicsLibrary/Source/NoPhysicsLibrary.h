#pragma once

#include "AuxiliarsHeader.h"
// Includes: Body.h, Audio.h 

#include "ScenarioPresetEnum.h"
#include "PhysicsPresetEnum.h"
#include "InUnitsEnum.h"

#include "SoundData.h"
#include "Collision.h"

class NPL
{
public:

	NPL();

	~NPL();

	// Initialize the library
	void Init(float pixelsPerMeter);

	// Clean Up and reset the library
	void CleanUp();

	// Create a new body
	//  - Follow this function with a "." to choose the body type
	//  - Rect must be on pixels (use MetersToPixels() to do the conversion)
	BodyCreation CreateBody(Rect rectangle, float mass);

	// Configure certain aspects of this library operation
	LibraryConfig Configure();

	// Iterates the library
	//  - Call it in your main update function
	void Update(float* dt);

	// Set a predesigned scenario
	//  - This function destroys automatically the previous scenario
	StaticBody* SetScenarioPreset(ScenarioPreset preset, Point windowSize = { 1080.0f, 720.0f }, int returnStatic = -1);

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

	// Returns the global friction vector
	Point GetGlobalFriction() const;

	// Returns the global restitution vector
	Point GetGlobalRestitution() const;

	// Destroys a body whenever it's outside the rectangle setted. 
	//    Returns true when a body is destroyed
	bool DeathLimit(Rect limits);
	bool DeathLimit(Rect limits, DynamicBody* body);

	// Destroy a body, returns true if the body has been successfully deleted
	bool DestroyBody(Body* body);

	// Load internally an audio file
	void LoadSound(const char* path);

	const Collision* GetCollisionsIterable(int &size, int index);

	const Body* GetBodiesIterable(int& size, int index);

	// Converts meter values into pixels values
	Rect MetersToPixels(Rect rectangle);
	Point MetersToPixels(Point rectangle);
	float MetersToPixels(float rectangle);


private: // Methods

	void StepPhysics(float dt);

	void StepAcoustics();
		void ListenerLogic(Body* b, GasBody* environment);
			float ComputePanning(float distance, float bodyX);
			float ComputeVolume(float distance, float spl);
			float ComputeTimeDelay(float distance, GasBody* environment);
		void NoListenerLogic(Body* b);

	void UpdateStates();

	void StepAudio(float* dt);

	bool EraseBody(Body* body);

	inline bool IsVoid() const { return gasIndex.empty(); }
	inline bool IsDry() const { return liquidIndex.empty(); }

	GasBody* GetEnvironmentBody(Rect body);

	void UpdateNotifier();
		void UpdatePixelsToMeters();

private:

	Physics* physics = nullptr;
	Audio* audio = nullptr;

	std::vector<Body*> bodies;
	std::vector<SoundData*> soundDataList;
	//INFO: Fet aixi perqu� amb unic ids s'hauria de fer moltes iteracions + protegir el vector de bodies
	std::vector<unsigned int*> gasIndex;
	std::vector<unsigned int*> liquidIndex;

	// CONSTANTS
	const float maxSPL = 120.0f;
	const float maxVolume = 10.0f;

	//_____________________
	// CONFIG VARIABLES
	// - Physics

	// Debug Collisions (0) |
	Flag physicsConfig = {};
	// Debug states (0) |
	Flag bodiesConfig = {};

	//_____________________
	// - Acoustics
	float panRange = 10.0f;
	float pixelsToMeters = 20.0f;
	float ptmRatio = 1.0f;

	//_____________________
	// - Audio
	Body* listener = nullptr;

	//______Config Notifier______
	// PixelsToMeters (0) |
	Flag notifier = {};

};