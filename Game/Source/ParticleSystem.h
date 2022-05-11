#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include "Module.h"

#include "Point.h"
#include "Emitter.h"
#include "Render.h"
#include <vector>
#include <string>
#include "SDL/include/SDL.h"

#define MAX_NUM_EMITTERS_TYPE 11

class Emitter;
struct SDL_Texture;
struct SDL_Rect;
struct SDL_Color;

enum EmitterType
{
	EMITTER_TYPE_NONE = -1,
	EMITTER_TYPE_FIRE,
	EMITTER_TYPE_FIRE_PURPLE,
	EMITTER_TYPE_FLAME,
	EMITTER_TYPE_SMOKE,
	EMITTER_TYPE_PIXEL_SMOKE,
	EMITTER_TYPE_BURST,
	EMITTER_TYPE_WAVE_1,
	EMITTER_TYPE_WAVE_2,
	EMITTER_TYPE_BUBBLE,
	EMITTER_TYPE_SPARK
};

struct EmitterData
{
	Point angleRange = { 0.0f, 0.0f };
	float startSpeed = 0.0f, endSpeed = 0.0f;
	float startSize = 0.0f, endSize = 0.0f;
	uint emitNumber = 0u;
	uint emitVariance = 0u;
	uint maxParticleLife = 0u;
	SDL_Rect textureRect = { 0, 0 };
	double lifetime = -1.0f;
	SDL_Color startColor = { 0, 0, 0, 0 };
	SDL_Color endColor = { 0, 0, 0, 0 };
	SDL_BlendMode blendMode = SDL_BlendMode::SDL_BLENDMODE_NONE;
	double rotSpeed = 0;

	// Randoms
	Point rotSpeedRand = { 0.0f, 0.0f };
	Point startSpeedRand = { 0.0f, 0.0f };
	Point endSpeedRand = { 0.0f, 0.0f };
	Point emitVarianceRand = { 0.0f, 0.0f };
	Point lifeRand = { 0.0f, 0.0f };
	Point startSizeRand = { 0.0f, 0.0f };
	Point endSizeRand = { 0.0f, 0.0f };

	// Vortex
	bool vortexSensitive = false;
};

class ParticleSystem : public Module
{
public:

	ParticleSystem(Render* render);

	// Destructor
	virtual ~ParticleSystem();

	// Call before first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Will be inexistent
	void PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Emitter methods
	//pugi::xml_node LoadEmitters(pugi::xml_document& psystem_file);
	Emitter* AddEmiter(Point pos, EmitterType type);
	bool RemoveEmitter(Emitter* emitter);
	bool RemoveAllEmitters();

	SDL_Texture* GetParticleAtlas() const;
	//void LoadEmitterData(pugi::xml_node& config, EmitterType type);

	Render* render = nullptr;

private:

	std::vector<Emitter*> emittersList;
	std::vector<Emitter*> emittersToDestroy;
	SDL_Texture* particleAtlas = nullptr;
	std::string nameParticleAtlas;

	// Static array that stores all the data of emitters
	EmitterData vecEmitterData[MAX_NUM_EMITTERS_TYPE];
};

#endif // __PARTICLE_SYSTEM_H__