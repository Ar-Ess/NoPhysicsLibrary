#ifndef __ASSETSMANAGER_H__
#define __ASSETSMANAGER_H__

#include "Module.h"

struct SDL_RWops;

class AssetsManager : public Module
{
public:

	AssetsManager();

	virtual ~AssetsManager();

	bool Start();

	bool Cleanup();

	SDL_RWops* LoadAsset(const char* path);
	size_t LoadXML(const char* path, char** buffer);

private:

public:

private:

};

#endif // __ASSETSMANAGER_H__
