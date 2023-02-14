#pragma once
#include "PhysRect.h"
#include "InUnitsEnum.h"
#include "DynamicBody.h"

struct Collision
{
	Collision(DynamicBody* dynamicBody, Body* body, PhysRect intersecRect, const float* pixelsToMeters) : dynamicBody(dynamicBody), body(body), intersecRect(intersecRect), pixelsToMeters(pixelsToMeters) {}
	
	const PhysRect GetCollisionRectangle(InUnit unit) const
	{
		PhysRect inter = intersecRect;

		if (unit == InUnit::IN_PIXELS)
		{
			inter = {inter.Position() * (1 / *pixelsToMeters), inter.Size() * (1 / *pixelsToMeters) };
		}

		return inter;
	}

	const Body* GetDynBody() const
	{
		return dynamicBody;
	}
	const Body* GetBody() const
	{
		return body;
	}

private:

	DynamicBody* dynamicBody = nullptr;
	Body* body = nullptr;
	PhysRect intersecRect = {};
	const float* pixelsToMeters = nullptr;

};