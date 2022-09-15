#pragma once

#include "External\dr_libs\dr_mp3.h"
#include "Sound.h"
#include "SoundData.h"
#include "Define.h"
#include <vector>
#include "Point.h"

class Audio
{
public:

	Audio();

	~Audio();

	void Update(SoundData* data);

	//-TODO: Aquesta funció és la de play audio dels bodies, treurela d'aquí
	void PushSound(int index, Point position);

	void LoadSound(const char* path);

	void CleanUp();

private:

	ma_engine engine;
	std::vector<Sound*> sounds;

};
