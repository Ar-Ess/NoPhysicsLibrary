#pragma once
#include "Rect.h"

struct Collision
{
	Collision(Body* dynamicBody, Body* body, Rect intersecRect) : dynamicBody(dynamicBody), body(body), intersecRect(intersecRect) {}
	
	const Rect GetCollisionRectangle() const
	{
		return intersecRect;
	}

	Body* GetDynBody()
	{
		return dynamicBody;
	}
	Body* GetBody()
	{
		return body;
	}

private:

	Body* dynamicBody = nullptr;
	Body* body = nullptr;
	Rect intersecRect = {};

};