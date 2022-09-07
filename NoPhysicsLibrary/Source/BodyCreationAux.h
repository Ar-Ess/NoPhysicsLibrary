#pragma once

#include "Physics.h"
#include "Audio.h"
#include "StaticBody.h"
#include "DynamicBody.h"
#include "LiquidBody.h"
#include "GasBody.h"

struct BodyCreation
{
	BodyCreation(Rect rect, float mass, Physics* physics, Audio* audio)
	{
		this->mass = mass;
		this->rect = rect;
		this->physics = physics;
		this->audio = audio;
	}

	StaticBody* Static()
	{
		StaticBody* body = new StaticBody(rect, mass);
		physics->bodies.push_back(body);
		return body;
	}

	DynamicBody* Dynamic(Point velocity = { 0, 0 }, Point gravity = {0, 0})
	{
		
		DynamicBody* body = new DynamicBody(rect, velocity, gravity, mass, &physics->globals, audio);
		physics->bodies.push_back(body);
		return body;
	}

	LiquidBody* Liquid()
	{
		LiquidBody* body = new LiquidBody(rect, mass);
		physics->bodies.push_back(body);
		return body;
	}

	GasBody* Gas()
	{
		GasBody* body = new GasBody(rect, mass);
		physics->bodies.push_back(body);
		return body;
	}

private:

	float mass = 1.0f;
	Rect rect = {};
	Physics* physics = nullptr;
	Audio* audio = nullptr;
};
