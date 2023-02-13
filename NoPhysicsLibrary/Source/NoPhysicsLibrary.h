#pragma once

#include "LibraryAuxiliars.h"
#include "LibraryEnums.h"

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
	//  - Rect must be on pixels (Multiply meters by "metersToPixels" variable on npl->Get() function)
	const BodyCreation* CreateBody(PhysRect rectangle);
	const BodyCreation* CreateBody(float x, float y, float width, float height);

	// Configure certain aspects of this library operation
	const LibraryConfig* Configure();

	const GetData* Get();

	// Iterates the library
	//  - Call it in your main update function
	void Update(float* dt);

	// Allows to pause or unpause the physics
	void PausePhysics(bool pause);

	// Destroy a body, returns true if the body has been successfully deleted
	bool DestroyBody(Body* body);
	// Destroy a body, returns true if the body has been successfully deleted
	bool DestroyBody(int id);
	// Destroy all bodies of the inputted class, returns true if all bodies has been successfully deleted
	bool DestroyBody(BodyClass clas);

	// Load internally an audio file
	void LoadSound(const char* path);

	// Calculates & returns a rectsngle that englobes all the existent bodies
	const PhysRect ReturnScenarioRect() const;

	// Sets a scenario preset. WindowSize variable must be in pixels.
	// Provide a std::vector pointer to get the scenario bodies
	void SetScenarioPreset(ScenarioPreset preset, PhysVec windowSize, std::vector<StaticBody*>* scenarioBodies = nullptr);

	void SetPhysicsPreset(PhysicsPreset preset);

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

	bool EraseBody(int id, Body* outBody = nullptr);

	inline bool IsVoid() const { return gasIndex.empty(); }
	inline bool IsDry() const { return liquidIndex.empty(); }

	GasBody* GetEnvironmentBody(PhysRect body);

	void UpdateNotifier();
		void UpdatePixelsToMeters();

	bool IsGlobalPause() { return physics->globals.Get(0); }

private:

	BodyCreation* bodyCreation = nullptr;
	LibraryConfig* libraryConfig = nullptr;
	GetData* getData = nullptr;

	Physics* physics = nullptr;
	Audio* audio = nullptr;

	std::vector<Body*> bodies;
	std::vector<SoundData*> soundDataList;
	//INFO: Fet aixi perqu� amb unic ids s'hauria de fer moltes iteracions + protegir el vector de bodies
	std::vector<unsigned int*> gasIndex;
	std::vector<unsigned int*> liquidIndex;
	PhysRect scenarioRects[4] = {};

	const float maxSPL = 120.0f;
	const float maxVolume = 10.0f;

	//_____________________
	// CONFIG VARIABLES
	// - Physics

	Flag physicsConfig = {}; // Debug Collisions (0) |
	unsigned int physIterations = 40;

	//_____________________
	// - Acoustics
	float panRange = 10.0f;
	float pixelsToMeters = 20.0f;
	float ptmRatio = 1.0f;

	//_____________________
	// - Audio
	Body* listener = nullptr;

	//______Config Notifier______
	Flag notifier = {}; // PixelsToMeters (0)

};