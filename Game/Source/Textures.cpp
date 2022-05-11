#include "App.h"
#include "AssetsManager.h"
#include "Render.h"
#include "Textures.h"

#include "Defs.h"
#include "Log.h"

#include "SDL_image/include/SDL_image.h"

Textures::Textures(Render* render, AssetsManager* assets) : Module()
{
	this->render = render;
	this->assets = assets;
}

Textures::~Textures()
{
}

bool Textures::Start()
{
	LOG("start textures");
	bool ret = true;

	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

bool Textures::CleanUp()
{
	LOG("Freeing textures and Image library");

	suint size = textures.size();
	for (suint i = 0; i < size; ++i) SDL_DestroyTexture(textures[i]);

	textures.clear();
	IMG_Quit();
	return true;
}

SDL_Texture* const Textures::Load(const char* path, Point* size)
{
	SDL_Texture* texture = nullptr;

	SDL_RWops* rW = assets->LoadAsset(path);
	SDL_Surface* surface = IMG_Load_RW(rW, 0);

	if (surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = LoadSurface(surface);
		if (size)
		{
			size->x = surface->w;
			size->y = surface->h;
		}
		SDL_FreeSurface(surface);
	}

	if (rW) SDL_RWclose(rW);

	return texture;
}

bool Textures::UnLoad(SDL_Texture* texture)
{
	suint size = textures.size();

	for (suint i = 0; i < size; ++i)
	{
		if (texture == textures[i])
		{
			textures.erase(textures.begin() + i);
			break;
		}
	}

	SDL_DestroyTexture(texture);
	texture = nullptr;

	return true;
}

SDL_Texture* const Textures::LoadSurface(SDL_Surface* surface, SDL_Texture* last)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(render->renderer, surface);

	if (!texture) 
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	else
		textures.push_back(texture);

	if (last)
	{
		UnLoad(last);
		last = nullptr;
	}

	return texture;
}

Point Textures::GetSize(const SDL_Texture* texture) const
{
	int width, height;
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, &width, &height);

	return Point{ width, height };
}