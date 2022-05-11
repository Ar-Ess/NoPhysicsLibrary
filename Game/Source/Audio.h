#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"
#include "AssetsManager.h"

#include "Point.h"

#include <vector>

#define MAX_CHANNELS			360.0f
#define MAX_FX					100.0f

#define MAX_DISTANCE			500.0f
#define MAX_DISTANCE_VOL		250.0f

#define MUTE_DISTANCE			510.0f
#define MUTE_DISTANCE_VOL		255.0f

#define RADS_TO_DEG				180 / 3.14

#define DEFAULT_MUSIC_FADE_TIME 1.0f

#define Y_AXIS {0, 1}

struct _Mix_Music;
struct Mix_Chunk;

enum Track
{
	NO_TRACK = -1,
};

enum Sfx
{
	NO_SFX = -1,
	BUTTON_FOCUSSED,
	BUTTON_RELEASED,
	SKIP_DIALOGUE
};

class Audio : public Module
{
public:

	Audio(AssetsManager* assets);

	virtual ~Audio();

	bool Start();

	bool CleanUp();

	bool LoadTrack(const char* path);
	bool LoadSfx(const char* path);
	bool PlayTrack(Track track, float fadeTime = DEFAULT_MUSIC_FADE_TIME);
	bool PlaySfx(Sfx fx);

	//Tools
	uint GetAngle(Point player, Point enemy);
	uint GetVolumeFromDistance(Point player, Point enemy);
	void SetChannelAngles();
	void ChangeVolumeMusic(suint volume);
	void ChangeVolumeFx(suint index, suint volume);
	void TogglePauseMusic();
	void StopMusic();
	int ValueToVolume(int value, int maxPercent = 100);
	int VolumeToValue(int volume, int maxPercent = 100);

	// GETTERS
	int GetMusicVolume();
	int GetFxVolume(suint index);

private:

	bool PlayFxOnChannel(uint id, uint channel, uint distance = 1, int repeat = 0);

private:

	std::vector<_Mix_Music*> music;
	std::vector<Mix_Chunk*> sfx;

	AssetsManager* assets = nullptr;
};

#endif // __AUDIO_H__