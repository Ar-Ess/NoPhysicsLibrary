#pragma once

#include "Physics.h"
#include "Audio.h"
#include "StaticBody.h"
#include "DynamicBody.h"
#include "LiquidBody.h"
#include "GasBody.h"

struct BodyCreation
{
	BodyCreation(Rect rect, float mass, std::vector<Body*>* bodies, Physics* physics, Audio* audio)
	{
		this->mass = mass;
		this->rect = rect;
		this->bodies = bodies;
		this->physics = physics;
		this->audio = audio;
	}

	StaticBody* Static()
	{
		StaticBody* body = new StaticBody(rect, mass);
		bodies->emplace_back(body);
		return body;
	}

	DynamicBody* Dynamic(Point velocity = { 0, 0 }, Point gravityOffset = {0, 0})
	{
		
		DynamicBody* body = new DynamicBody(rect, velocity, gravityOffset, mass, &physics->globals, audio);
		bodies->emplace_back(body);
		return body;
	}

	LiquidBody* Liquid()
	{
		LiquidBody* body = new LiquidBody(rect, mass);
		bodies->emplace_back(body);
		return body;
	}

	GasBody* Gas()
	{
		GasBody* body = new GasBody(rect, mass);
		bodies->emplace_back(body);
		return body;
	}

private:

	float mass = 1.0f;
	Rect rect = {};
	std::vector<Body*>* bodies = nullptr;
	Physics* physics = nullptr;
	Audio* audio = nullptr;
};