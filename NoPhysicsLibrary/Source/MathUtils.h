#pragma once

#include "Rect.h"

namespace MathUtils
{
	inline float Distance(float x1, float y1, float x2, float y2)
	{
		double dx = double(x2) - double(x1);
		double dy = double(y2) - double(y1);
		return float(sqrt(pow(dx, 2) + pow(dy, 2)));
	}

	inline bool CheckCollision(Rect r1, Rect r2)
	{
		return !((r1.x > r2.x + r2.w) || (r1.x + r1.w < r2.x) || (r1.y > r2.y + r2.h) || (r1.y + r1.h < r2.y));
	}

	inline float Ln(float num)
	{
		const double x = (num - 1) / (num + 1);
		long double r = 0;
		for (long long n = 0; n <= 100; ++n)
		{
			r += 2 * pow(x, 2 * n + 1) / (2 * n + 1);
		}
		return r;
	}

	inline float Sqrt(float num) 
	{
		float x2 = num * 0.5f, y = num;

		long int i = *(long*)&y;

		i = 0x5f3759df - (i >> 1);

		y = *(float*)&i;

		y = y * (1.5f - (x2 * y * y));
		y = y * (1.5f - (x2 * y * y));

		return y;
	}

	//// Calculate 2 Neperian Logarithms at the same time
	inline Point DoubleLn(float num1, float num2)
	{
		const double x1 = (num1 - 1) / (num1 + 1);
		const double x2 = (num2 - 1) / (num2 + 1);
		long double r1 = 0, r2 = 0;
		for (long long n = 0; n <= 100; ++n)
		{
			r1 += 2 * pow(x1, 2 * n + 1) / (2 * n + 1);
			r2 += 2 * pow(x2, 2 * n + 1) / (2 * n + 1);
		}
		return Point{(float)r1, (float)r2};
	}

	inline float Log(float num, float base)
	{
		Point ln = DoubleLn(num, base);
		return ln.x / ln.y;
	}

	inline int Min(int a, int b)
	{
		if (a <= b) return a;
		return b;
	}

	inline float Min(float a, float b)
	{
		if (a <= b) return a;
		return b;
	}

	inline int Max(int a, int b)
	{
		if (a >= b) return a;
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

		return { x, y, w, h };
	}

};
