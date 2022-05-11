#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Module.h"

#include "Point.h"

struct SDL_Window;
struct SDL_Surface;

class Window : public Module
{
public:

	Window();

	virtual ~Window();

	bool Start();

	bool CleanUp();

// Getters & Setters

	void SetTitle(const char* title);

	suint GetScale() const
	{
		return scale;
	}

	void SetWinFullScreen(bool fullScreen);

	void SetWinDFullScreen(bool fullScreen);

	Point GetSize() const { return size; }

public:

	SDL_Window* window;

	SDL_Surface* screenSurface;

	bool fullScreen = false;
	bool dFullScreen = false;

private:

	Point size;
	suint scale = 1;
};

#endif // __WINDOW_H__