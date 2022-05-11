#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include "Module.h"
#include "Render.h"
#include "AssetsManager.h"

#include "Point.h"
#include <vector>
#include <string>

struct SDL_Texture;
struct SDL_Surface;

class Textures : public Module
{
public:

	Textures(Render* render, AssetsManager* assets);

	virtual ~Textures();

	bool Start();

	bool CleanUp();

	SDL_Texture* const Load(const char* path, Point* size = nullptr);
	SDL_Texture* const LoadSurface(SDL_Surface* surface, SDL_Texture* last = nullptr);
	bool UnLoad(SDL_Texture* texture);
	Point GetSize(const SDL_Texture* texture) const;

private:

	AssetsManager* assets = nullptr;
	Render* render = nullptr;

	std::vector<SDL_Texture*> textures;
};


#endif // __TEXTURES_H__