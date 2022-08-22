#pragma once
#include <vector>
#include "StaticBody.h"
#include "DynamicBody.h"
#include "LiquidBody.h"
#include "GasBody.h"

struct BodyCreation
{
	BodyCreation(Rect rect, float mass, std::vector<Body*>* bodies)
	{
		this->mass = mass;
		this->rect = rect;
		this->bodies = bodies;
	}

	StaticBody* Static()
	{
		StaticBody* body = new StaticBody(rect, mass);
		bodies->push_back(body);
		return body;
	}

	DynamicBody* Dynamic(Point velocity = { 0, 0 }, Point gravity = {0, 0})
	{
		DynamicBody* body = new DynamicBody(rect, velocity, gravity, mass);
		bodies->push_back(body);
		return body;
	}

	LiquidBody* Liquid()
	{
		LiquidBody* body = new LiquidBody(rect, mass);
		bodies->push_back(body);
		return body;
	}

	GasBody* Gas()
	{
		GasBody* body = new GasBody(rect, mass);
		bodies->push_back(body);
		return body;
	}

private:

	float mass = 1.0f;
	Rect rect = {};
	std::vector<Body*>* bodies = nullptr;
};
