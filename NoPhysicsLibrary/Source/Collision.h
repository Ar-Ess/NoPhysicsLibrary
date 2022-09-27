#pragma once
#include "Rect.h"

struct Collision
{
	Collision(Body* dynamicBody, Body* body, Rect intersecRect) : dynamicBody(dynamicBody), body(body), intersecRect(intersecRect) {}

	Body* dynamicBody = nullptr;
	Body* body = nullptr;
	Rect intersecRect = {};

};