#pragma once
#include "PhysVec.h"

struct PhysRect
{
	PhysRect() {}
	PhysRect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
	PhysRect(int   x, int   y, int   w, int   h) : x(x), y(y), w(w), h(h) {}
	PhysRect(float x, float y, PhysVec size) : x(x), y(y), w(size.x), h(size.y) {}
	PhysRect(int   x, int   y, PhysVec size) : x(x), y(y), w(size.x), h(size.y) {}
	PhysRect(PhysVec position, float w, float h) : x(position.x), y(position.y), w(w), h(h) {}
	PhysRect(PhysVec position, int   w, int   h) : x(position.x), y(position.y), w(w), h(h) {}
	PhysRect(PhysVec position, PhysVec size) : x(position.x), y(position.y), w(size.x), h(size.y) {}

	PhysVec Position() const;
	void Position(float x, float y);
	void Position(PhysVec position);

	PhysVec Size() const;
	void Size(float w, float h);
	void Size(PhysVec size);

	float x = 0, y = 0, w = 0, h = 0;
};
