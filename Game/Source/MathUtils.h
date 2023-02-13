#pragma once

#include "Rect.h"
#include "Ray.h"
#define PI 3.14159265359

namespace MathUtils
{
	// This function does not consider a collision if rects touches their bounds
	inline bool CheckCollision(Rect r1, Rect r2)
	{
		return !((r1.x >= r2.x + r2.w) || (r1.x + r1.w < r2.x) || (r1.y >= r2.y + r2.h) || (r1.y + r1.h < r2.y));
	}

	inline float Sqrt(float num)
	{
		if (num <= 0) return -1;

		return (float)sqrt((double)num);
	}

	inline float Pow(float num, float power)
	{
		if (power == 1) return num;
		else if (power == 2) return num * num;
		else if (power == 3) return num * num * num;
		else if (power == 4) return num * num * num * num;
		else if (power == 5) return num * num * num * num * num;

		return (float)pow((double)num, (double)power);
	}

	inline float RadToDeg(float rad)
	{
		return rad * (180 / PI);
	}

	inline float DegToRad(float deg)
	{
		return deg * (PI / 180);
	}

	inline int Ceil(float num)
	{
		return (int)ceilf(num);
	}

	inline int Floor(float num)
	{
		return (int)floorf(num);
	}

	inline float Sin(float num, bool radians = true)
	{
		if (radians) return sin(num);
		
		return sin(RadToDeg(num));
	}

	inline float Cos(float num, bool radians = true)
	{
		if (radians) return cos(num);

		return cos(RadToDeg(num));
	}

	inline float Distance(float x1, float y1, float x2, float y2)
	{
		double dx = double(x2) - double(x1);
		double dy = double(y2) - double(y1);
		return float(sqrt((double)Pow((float)dx, 2) + (double)Pow((float)dy, 2)));
	}

	inline float Ln(float num)
	{
		return (float)log((double)num);
	}

	inline float Log(float num, float base = 10)
	{
		if (base == 10) return (float)log10((double)num);

		return Ln(num) / Ln(base);
	}

	inline float LogToLinear(float num, float target)
	{
		return Pow(10, num / target);
	}

	inline float LinearToLog(float num)
	{
		return 20 * Log(num, 10);
	}

	inline float Min(float a, float b)
	{
		if (a <= b) return a;
		return b;
	}

	inline float Max(float a, float b)
	{
		if (a >= b) return a;
		return b;
	}

	inline float Abs(float num)
	{
		if (num < 0) num *= -1;
		return num;
	}

	inline void Clamp(float& value, float min, float max)
	{
		if (value < min) value = min;
		else if (value > max) value = max;
	}

	inline Rect IntersectRectangle(Rect r1, Rect r2)
	{
		float x = Max(r1.x, r2.x);
		float y = Max(r1.y, r2.y);
		float w = Min(r1.x + r1.w, r2.x + r2.w) - x;
		float h = Min(r1.y + r1.h, r2.y + r2.h) - y;

		if (w <= 0 || h <= 0) return {0, 0, 0, 0};

		return { x, y, w, h };
	}
	
	inline Point ClosestRectIntersectionFromOutsidePoint(Point point, Rect rect)
	{
		Point ret = { 0.0f, 0.0f };

		ret.x = Max(rect.x, Min(point.x, rect.x + rect.w));
		ret.y = Min(rect.y + rect.h, Max(point.y, rect.y));

		return ret;
	}

	inline Point ClosestRectIntersectionFromInsidePoint(Point point, Rect rect)
	{
		Point ret = { 0.0f, 0.0f };

		Point distanceToPositiveBounds = rect.GetPosition(Align::TOP_RIGHT) - point;
		Point distanceToNegativeBounds = rect.GetPosition(Align::BOTTOM_LEFT) - point;

		float smallestX = Min(distanceToPositiveBounds.x, distanceToNegativeBounds.x);
		float smallestY = Min(distanceToPositiveBounds.y, distanceToNegativeBounds.y);
		float smallestDistance = Min(smallestX, smallestY);

		if (smallestDistance == distanceToPositiveBounds.x) ret = { rect.x + rect.w, point.y };
		else if (smallestDistance == distanceToNegativeBounds.x) ret = { rect.x, point.y };
		else if (smallestDistance == distanceToPositiveBounds.y) ret = { point.x, rect.y };
		else if (smallestDistance == distanceToNegativeBounds.y) ret = { point.x, rect.y + rect.h };

		return ret;
	}

	inline Point ClosestRectIntersectionFromPoint(Point point, Rect rect)
	{
		bool inside = MathUtils::CheckCollision({ point.Apply(-1.0f, -1.0f), 2, 2 }, rect);
		Point ret = { 0.0f, 0.0f };

		inside ? ret = MathUtils::ClosestRectIntersectionFromInsidePoint(point, rect) : ret = MathUtils::ClosestRectIntersectionFromOutsidePoint(point, rect);

		return ret;
	}

};
