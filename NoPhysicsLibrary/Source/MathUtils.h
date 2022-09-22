#pragma once

#include "Rect.h"

namespace MathUtils
{

	inline bool CheckCollision(Rect r1, Rect r2)
	{
		return !((r1.x > r2.x + r2.w) || (r1.x + r1.w < r2.x) || (r1.y > r2.y + r2.h) || (r1.y + r1.h < r2.y));
	}

	inline float Sqrt(float num)
	{
		if (num <= 0) return -1;

		return sqrt(num);
	}

	inline float Pow(float num, float power)
	{
		if (power == 2) return num * num;

		return pow(num, power);
	}

	inline float Distance(float x1, float y1, float x2, float y2)
	{
		double dx = double(x2) - double(x1);
		double dy = double(y2) - double(y1);
		return float(sqrt(Pow(dx, 2) + Pow(dy, 2)));
	}

	inline float Ln(float num)
	{
		return log(num);
	}

	inline float Log(float num, float base = 10)
	{
		if (base == 10) return log10(num);

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

	inline Rect IntersectRectangle(Rect r1, Rect r2)
	{
		float x = Max(r1.x, r2.x);
		float y = Max(r1.y, r2.y);
		float w = Min(r1.x + r1.w, r2.x + r2.w) - x;
		float h = Min(r1.y + r1.h, r2.y + r2.h) - y;

		if (w <= 0 || h <= 0) return {0, 0, 0, 0};

		return { x, y, w, h };
	}

};
