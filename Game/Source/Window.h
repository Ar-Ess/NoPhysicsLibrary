#pragma once

#include "Module.h"

#include "SDL/include/SDL.h"
#include "Point.h"

//struct SDL_Window;
//struct SDL_Surface;

class Window : public Module
{
public:

	Window();

	virtual ~Window();

	bool Awake();

	bool Start();

	bool CleanUp();

// Getters & Setters

	void SetTitle(const char* title);

	inline suint GetScale() const { return scale; }

	void SetWinFullScreen(bool fullScreen);

	void SetWinDFullScreen(bool fullScreen);

	inline Point GetSize() const { return size; }

public:

	SDL_Window* window;
	SDL_Surface* screenSurface;

	bool fullScreen = false;
	bool dFullScreen = false;

private:

	Point size;
	suint scale = 1;
};