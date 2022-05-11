#include "App.h"

#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "AssetsManager.h"
#include "Scene.h"

#include <time.h>

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

App::App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	win = new Window();
	input = new Input(win);
	assetsManager = new AssetsManager();
	render = new Render(win);
	tex = new Textures(render, assetsManager);
	audio = new Audio(assetsManager);
	scene = new Scene(render, input, tex, win, audio);

	AddModule(win);
	AddModule(input);
	AddModule(assetsManager);
	AddModule(tex);
	AddModule(audio);
	AddModule(scene);
	AddModule(render);
	
	PERF_PEEK(ptimer);
}

App::~App()
{
	suint size = modules.size();
	for (suint i = 0; i < size; ++i)
	{
		Module* m = modules[i];
		RELEASE(m);
	}

	modules.shrink_to_fit();
	modules.clear();
}

void App::AddModule(Module* module)
{
	modules.push_back(module);
}

bool App::Start()
{
	bool ret = true;
	PERF_START(ptimer);

	suint size = modules.size();
	for (suint i = 0; i < size; ++i) ret = modules[i]->Start();
	
	PERF_PEEK(ptimer);

	srand(time(NULL));

	return ret;
}

void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	dt = frameTime.ReadSec();
	frameTime.Start();
}

bool App::Update()
{
	bool ret = true;

	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT)) ret = false;

	suint size = modules.size();
	for (suint i = 0; i < size; ++i)
	{
		Module* m = modules[i];
		if (!m->active) continue;

		if (!m->PreUpdate(dt)) ret = false;
	}

	for (suint i = 0; i < size; ++i)
	{
		Module* m = modules[i];
		if (!m->active) continue;

		if (!m->Update(dt)) ret = false;
	}

	FinishUpdate();

	return ret;
}

void App::FinishUpdate()
{
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();
    
	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	float averageFps = float(frameCount) / startupTime.ReadSec();
	float secondsSinceStartup = startupTime.ReadSec();
	uint32 lastFrameMs = frameTime.Read();
	uint32 framesOnLastUpdate = prevLastSecFrameCount;

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %I64u ",
			  averageFps, lastFrameMs, framesOnLastUpdate, dt, secondsSinceStartup, frameCount);

	win->SetTitle(title);

	if (frameDelay > lastFrameMs)
	{
		SDL_Delay(frameDelay - lastFrameMs);
	}
}

bool App::CleanUp()
{
	bool ret = true;

	suint size = modules.size();
	for (suint i = 0; i < size; ++i)
	{
		Module* m = modules[i];

		modules[i]->CleanUp();
	}

	return ret;
}

int App::GetArgc() const
{
	return argc;
}

const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

void App::LoadGameRequest()
{
	loadGameRequested = true;
}

void App::SaveGameRequest() const
{
	saveGameRequested = true;
}

bool App::LoadGame()
{
	bool ret = true;

	return ret;
}

bool App::SaveGame() const
{
	bool ret = true;

	return ret;
}
