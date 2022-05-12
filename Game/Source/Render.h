#ifndef __RENDER_H__
#define __RENDER_H__

#include "Module.h"

#include "Point.h"
#include "SDL/include/SDL.h"
#include "Collision.h"
#include "Rect.h"
#include "Window.h"

class Render : public Module
{
public:

	Render(Window* win);

	virtual ~Render();

	bool Start();

	bool PreUpdate(float dt);

	bool Update(float dt);

	bool CleanUp();

public:

	// Utils
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();

	// Drawing
	bool DrawTexture(SDL_Texture* texture, Point position, Point size, bool anchored = false, Rect* section = nullptr, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
	bool DrawRectangle(Rect rect, SDL_Color color = { 255, 255, 255, 255 }, Point size = {1.0f, 1.0f}, bool anchored = true, bool fill = true) const;
	bool DrawLine(Point start, Point end, SDL_Color color = { 255, 255, 255, 255}, bool anchored = true) const;
	bool DrawCircle(CircleCollider circle, SDL_Color color, bool fill = false, bool anchored = true) const;
	bool DrawParticle(SDL_Texture* texture, Point position, Rect* section = nullptr, Rect* rectSize = nullptr, SDL_Color color = { 0, 0, 0, 0 }, SDL_BlendMode blendMode = SDL_BLENDMODE_NONE, double angle = 0) const;

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	// VSYNC Functions
	bool GetVSync() const { return vSync; }
	void SetVSync(bool vSync);

	// Size Functions
	suint GetScale() const { return scale; };
	void SetScale(suint scale) { this->scale = scale; }

public:

	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera = {};
	SDL_Rect viewport = {};
	SDL_Color background = {};

private:

	Window* win = nullptr;

	bool vSync = true;
	suint scale = 1;
};

#endif // __RENDER_H__