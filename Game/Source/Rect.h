#pragma once

#include "Point.h"
#include "Alignment.h"
#include "PhysRect.h"

class Rect
{
public:

	Rect(PhysRect rect)
	{
		this->x = rect.x;
		this->y = rect.y;
		this->w = rect.w;
		this->h = rect.h;
	}

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

	Rect(float x, float y, Point size)
	{
		this->x = x;
		this->y = y;
		this->w = size.x;
		this->h = size.y;
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

	Point GetPosition(Align alignment) const
	{
		Point ret = { 0.0f, 0.0f };

		switch (alignment)
		{
		case Align::CENTER: ret = { x + w / 2, y + h / 2 }; break;

		case Align::CENTER_LEFT: ret = { x, y + h / 2 }; break;

		case Align::CENTER_RIGHT: ret = { x + w, y + h / 2 }; break;

		case Align::TOP_CENTER: ret = { x + w / 2, y }; break;

		case Align::TOP_LEFT: ret = {x, y}; break;

		case Align::TOP_RIGHT: ret = { x + w, y }; break;

		case Align::BOTTOM_CENTER: ret = { x + w / 2, y + h }; break;

		case Align::BOTTOM_LEFT: ret = { x, y + h }; break;

		case Align::BOTTOM_RIGHT: ret = { x + w, y + h }; break;

		default: break;
		}

		return ret;
	}

	Point GetSize() const
	{
		return Point{ w, h };
	}

	float GetArea() const
	{
		return w * h;
	}

	// Returns if the rectancle does not exist (width = 0 ot height = 0)
	bool IsNull() const
	{
		return (w == 0 || h == 0);
	}

public:

	float x = 0, y = 0, w = 0, h = 0;
};