#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Point.h"
#include "Defs.h"
#include "SDL/include/SDL.h"
//#include "ParticleSystem.h"
//#include "Render.h"

#define MIN_LIFE_TO_INTERPOLATE 15

struct SDL_Texture;
class ParticleSystem;
class Render;

class Particle
{
private:

	/*  This is the only variable we care about no matter if
	   the particle is alive or dead */
	uint life = 0;

	struct Vortex
	{
		Point pos = { 0.0f, 0.0f };
		float speed = 0.0f;
		float scale = 0.0f;
	} vortex;

	union ParticleInfo
	{
		/* This struct holds the state of the particle when
		   it's being update (it's still alive).*/
		struct ParticleState
		{
			uint startLife;
			Point pos;
			Point startVel;
			Point endVel;
			Point currentVel;
			float currentSize, startSize, endSize;
			float ageRatio;
			float angle;
			double startRotSpeed;
			double currentRotSpeed;
			SDL_Rect pRect;
			SDL_Rect rectSize;
			SDL_Color startColor;
			SDL_Color endColor;
			SDL_BlendMode blendMode;
			bool vortexSensitive;
			float t;

			ParticleState() {}

		} pLive;

		/* If the particle is dead, then the 'next' member comes
		   into play and the struct it's not used. This pointer
		   called 'next' holds a pointer to the next available
		   particle after this one. */
		Particle* next;

		ParticleInfo() {}
	} pState;

	Render* render = nullptr;

	ParticleSystem* particles = nullptr;

public:

	Particle(Render* render, ParticleSystem* particle);

	// Initializes new generated particle
	void Init(Point pos, float startSpeed, float endSpeed, float angle, double rotSpeed, float startSize, float endSize, uint life, SDL_Rect textureRect, SDL_Color startColor, SDL_Color endColor, SDL_BlendMode blendMode, bool vortexSensitive);

	// Generic methods
	void Update();
	bool Draw();
	bool IsAlive();

	// Particle pointer methods
	Particle* GetNext();
	void SetNext(Particle* next);

	// Given two colors interpolates linearly over time and returns the resulting color
	SDL_Color RgbInterpolation(SDL_Color startColor, float timeStep, SDL_Color endColor);

	float InterpolateBetweenRange(float min, float timeStep, float max);

	// Adds a vortex to the system
	void AddVortex(Point pos, float speed, float scale);

	/* Calculates particle position considering its velocity
	   and if there's a vortex in the system */
	void CalculateParticlePos();
};

#endif