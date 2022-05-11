#ifndef __RENDER_H__
#define __RENDER_H__

#include "Module.h"

#include "Point.h"
#include "SDL/include/SDL.h"
#include "Collider.h"
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
	// Load / Save
	bool Load();
	bool Save() const;

	// Utils
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();

	// Drawing
	bool DrawTexture(SDL_Texture* texture, int x, int y, float sX, float sY, SDL_Rect* section = NULL, bool scaleModCoords = false, bool staticPos = true, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
	bool DrawTexture(SDL_Texture* texture, Point position, Point size, bool anchored = false, Rect* section = nullptr, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE) const;


	bool DrawRectangle(const SDL_Rect& rect, SDL_Color color, bool filled = true, bool useCamera = true) const;
	bool DrawRectangle(Rect rect, SDL_Color color = { 255, 255, 255, 255 }, Point size = {1.0f, 1.0f}, bool filled = true, bool useCamera = true) const;
	bool DrawLine(float x1, float y1, float x2, float y2, SDL_Color color = { 255, 255, 255, 255}, bool useCamera = true) const;
	bool DrawCircle(int x1, int y1, int radius, SDL_Color color, bool filled = false, bool useCamera = true) const;
	bool DrawCircle(CircleCollider circle, SDL_Color color, bool filled = false, bool useCamera = true) const;

	//bool DrawText(Font font, const char* text, int x, int y, int size, int spacing, SDL_Color tint);
	bool BlitParticle(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, const SDL_Rect* rectSize = NULL, SDL_Color color = { 0, 0, 0, 0 }, SDL_BlendMode blendMode = SDL_BLENDMODE_NONE, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	// VSYNC Functions
	bool GetVSync() const { return vSync; }
	void SetVSync(bool vSync);

	// Size Functions
	suint GetScale() const { return scale; };
	void SetScale(suint scale) { this->scale = scale; }

public:

	SDL_Renderer* renderer;
	SDL_Rect camera;
	SDL_Rect viewport;
	SDL_Color background;

private:

	Window* win = nullptr;

	bool vSync = true;
	suint scale;
};

#endif // __RENDER_H__