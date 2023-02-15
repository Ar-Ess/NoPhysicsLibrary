#pragma once

#include "Body.h"

struct Collision
{
	Collision(Body* dynamicBody, Body* body, PhysRect intersecRect, const float* pixelsToMeters) : dynamicBody(dynamicBody), body(body), intersecRect(intersecRect), pixelsToMeters(pixelsToMeters) {}
	
	const PhysRect GetCollisionRectangle(InUnit unit) const
	{
		PhysRect inter = intersecRect;

		if (unit == InUnit::IN_PIXELS)
		{
			inter = {inter.Position() * (1 / *pixelsToMeters), inter.Size() * (1 / *pixelsToMeters) };
		}

		return inter;
	}

	const Body* DynBody() const
	{
		return dynamicBody;
	}

	const Body* OtherBody() const
	{
		return body;
	}

private:

	Body* dynamicBody = nullptr;
	Body* body = nullptr;
	PhysRect intersecRect = {};
	const float* pixelsToMeters = nullptr;

};