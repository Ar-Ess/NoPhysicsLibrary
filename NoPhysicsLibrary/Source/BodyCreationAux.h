#pragma once
#include <vector>
#include "StaticBody.h"
#include "DynamicBody.h"

struct BodyCreation
{
	BodyCreation(Rect rect, float mass, std::vector<Body*>* bodies)
	{
		this->mass = mass;
		this->rect = rect;
		this->bodies = bodies;
	}
	
	Body* Empty()
	{
		Body* body = new Body(BodyClass::EMPTY_BODY, rect, mass);
		bodies->push_back(body);
		return body;
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

private:

	float mass = 1.0f;
	Rect rect = {};
	std::vector<Body*>* bodies = nullptr;
};
