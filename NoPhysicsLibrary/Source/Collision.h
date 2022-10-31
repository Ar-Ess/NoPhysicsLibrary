#pragma once
#include "Rect.h"
#include "InUnitsEnum.h"
#include "DynamicBody.h"

struct Collision
{
	Collision(DynamicBody* dynamicBody, Body* body, Rect intersecRect, const float* pixelsToMeters) : dynamicBody(dynamicBody), body(body), intersecRect(intersecRect), pixelsToMeters(pixelsToMeters) {}
	
	const Rect GetCollisionRectangle(InUnit unit) const
	{
		Rect inter = intersecRect;

		if (unit == InUnit::IN_PIXELS)
		{
			inter = {inter.GetPosition() * (1 / *pixelsToMeters), inter.GetSize() * (1 / *pixelsToMeters) };
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
	Rect intersecRect = {};
	const float* pixelsToMeters = nullptr;

};