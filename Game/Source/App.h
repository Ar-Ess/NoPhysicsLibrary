#pragma once

#include "Module.h"
#include "PerfTimer.h"
#include "Timer.h"

#include <vector>

// Modules
class Window;
class Input;
class Render;
class SceneManager;

class App
{
public:
	App(int argc, char* args[]);

	virtual ~App();

	bool Start();

	bool Update();

	bool CleanUp();

	void AddModule(Module* module);

	int GetArgc() const;

	const char* GetArgv(int index) const;

	inline uint64 GetFrameCount() const
	{
		return frameCount;
	}
	
	inline float GetDeltaTime() const
	{
		return dt;
	}

	void LoadGameRequest();

	void SaveGameRequest() const;

private:

	void PrepareUpdate();

	void FinishUpdate();

	bool LoadGame();
	bool SaveGame() const;

public:

	// Modules
	Window* win = nullptr;
	Input* input = nullptr;
	Render* render = nullptr;
	SceneManager* scene = nullptr;

private:

	int argc;
	char** args;

	std::vector<Module*> modules;

	mutable bool saveGameRequested;
	bool loadGameRequested;

	// L07: DONE 4: Calculate some timing measures
	// required variables are provided:
	PerfTimer ptimer;
	uint64 frameCount = 0;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;
	uint32 lastSecFrameCount = 0;
	uint32 prevLastSecFrameCount = 0;
	float dt = 0.0f;

	const uint32 frameDelay = 1000 / 60;

	int	cappedMs = -1;
};