#pragma once
#include "Rect.h"

struct Collision
{
	Collision(unsigned int body1, unsigned int body2, Rect intersecRect) : body1(body1), body2(body2), intersecRect(intersecRect) {}

	unsigned int body1 = 0, body2 = 0;
	Rect intersecRect = {};

};