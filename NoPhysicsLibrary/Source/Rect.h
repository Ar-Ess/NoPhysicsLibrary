#pragma once

#include "Point.h"

class Rect
{
public:
	Rect()
	{
		this->x = 0;
		this->y = 0;
		this->w = 0;
		this->h = 0;
	}

	Rect(float x, float y, float w, float h) 
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	Rect(Point position, float w = 0, float h = 0)
	{
		this->x = position.x;
		this->y = position.y;
		this->w = w;
		this->h = h;
	}

	Rect(Point position, Point size)
	{
		this->x = position.x;
		this->y = position.y;
		this->w = size.x;
		this->h = size.y;
	}

	void SetPosition(Point position)
	{
		x = position.x;
		y = position.y;
	}

	void SetDimensions(Point dimension)
	{
		w = dimension.x;
		h = dimension.y;
	}

	Point GetPosition() const
	{
		return Point{x, y};
	}

	Point GetCentricPosition() const
	{
		return Point{ x + w/2, y + h/2 };
	}

	Point GetSize() const
	{
		return Point{ w, h };
	}

	// Returns if the rectancle does not exist (width = 0 ot height = 0)
	bool IsNull() const
	{
		return (w == 0 || h == 0);
	}

public:

	float x = 0, y = 0, w = 0, h = 0;
};