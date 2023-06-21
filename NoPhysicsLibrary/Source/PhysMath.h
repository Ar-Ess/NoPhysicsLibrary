#pragma once
#include "PhysRect.h"
#include "Ray.h"
#include <math.h>

namespace PhysMath
{
	inline double Pi()
	{
		return 3.14159265359;
	}

	// This function does not consider a collision if rects touches their bounds
	inline bool CheckCollision(PhysRect r1, PhysRect r2)
	{
		return !((r1.x >= r2.x + r2.w) || (r1.x + r1.w < r2.x) || (r1.y >= r2.y + r2.h) || (r1.y + r1.h < r2.y));
	}

	inline float Module(float x, float y)
	{
		return (float)sqrt(double(x) * double(x) + double(y) * double(y));
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

	inline float Exp(float num)
	{
		return exp(num);
	}

	inline double E()
	{
		return Exp(1);
	}

	inline float RadToDeg(float rad)
	{
		return rad * float(180 / Pi());
	}

	inline float DegToRad(float deg)
	{
		return deg * float(Pi() / 180);
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
		if (radians) return (float)sin(num);
		
		return (float)sin(RadToDeg(num));
	}

	inline float Cos(float num, bool radians = true)
	{
		if (radians) return (float)cos(num);

		return (float)cos(RadToDeg(num));
	}

	inline float Distance(float x1, float y1, float x2, float y2)
	{
		double dx = double(x2) - double(x1);
		double dy = double(y2) - double(y1);
		return float(sqrt((double)Pow((float)dx, 2) + (double)Pow((float)dy, 2)));
	}

	inline float Distance(PhysVec p1, PhysVec p2)
	{
		double dx = double(p2.x) - double(p1.x);
		double dy = double(p2.y) - double(p1.y);
		return float(sqrt((double)Pow((float)dx, 2) + (double)Pow((float)dy, 2)));
	}

	inline float Distance(PhysRay r)
	{
		double dx = double(r.end.x) - double(r.start.x);
		double dy = double(r.end.y) - double(r.start.y);
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

	inline float LinearToLog(float num, float base = 10)
	{
		return Log(num, base);
	}

	inline float LinearToLogV2(float num, float minLinear, float maxLinear, float minLog, float maxLog)
	{
		float log = PhysMath::Log(minLog) + (PhysMath::Log(maxLog) - PhysMath::Log(minLog)) * (num - minLinear) / (maxLinear - minLinear);
		return PhysMath::Pow(10, log);
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

	inline float Clamp(float value, float min, float max, bool onlyMin)
	{
		if (value < min) value = min;
		else if (!onlyMin && value > max) value = max;
		return value;
	}

	inline void Clamp(PhysVec& value, float min, float max)
	{
		if (value.x < min) value.x = min;
		else if (value.x > max) value.x = max;
		if (value.y < min) value.y = min;
		else if (value.y > max) value.y = max;
	}

	inline PhysRect IntersectRectangle(PhysRect r1, PhysRect r2)
	{
		float x = Max(r1.x, r2.x);
		float y = Max(r1.y, r2.y);
		float w = Min(r1.x + r1.w, r2.x + r2.w) - x;
		float h = Min(r1.y + r1.h, r2.y + r2.h) - y;

		if (w <= 0 || h <= 0) return {0, 0, 0, 0};

		return { x, y, w, h };
	}
	
	inline PhysVec ClosestRectIntersectionFromOutsidePoint(PhysVec point, PhysRect rect)
	{
		PhysVec ret = { 0.0f, 0.0f };

		ret.x = Max(rect.x, Min(point.x, rect.x + rect.w));
		ret.y = Min(rect.y + rect.h, Max(point.y, rect.y));

		return ret;
	}

	inline PhysVec ClosestRectIntersectionFromInsidePoint(PhysVec point, PhysRect rect)
	{
		PhysVec ret = { 0.0f, 0.0f };

		PhysVec distanceToPositiveBounds = rect.Position() + PhysVec(rect.Size().x, 0.f) - point;
		PhysVec distanceToNegativeBounds = rect.Position() + PhysVec(0.f, rect.Size().y) - point;

		float smallestX = Min(distanceToPositiveBounds.x, distanceToNegativeBounds.x);
		float smallestY = Min(distanceToPositiveBounds.y, distanceToNegativeBounds.y);
		float smallestDistance = Min(smallestX, smallestY);

		if (smallestDistance == distanceToPositiveBounds.x) ret = { rect.x + rect.w, point.y };
		else if (smallestDistance == distanceToNegativeBounds.x) ret = { rect.x, point.y };
		else if (smallestDistance == distanceToPositiveBounds.y) ret = { point.x, rect.y };
		else if (smallestDistance == distanceToNegativeBounds.y) ret = { point.x, rect.y + rect.h };

		return ret;
	}

	inline PhysVec ClosestRectIntersectionFromPoint(PhysVec point, PhysRect rect)
	{
		float x = point.x - 1;
		float y = point.y - 1;
		bool inside = PhysMath::CheckCollision({ x, y, 2.0f, 2.0f }, rect);
		PhysVec ret = { 0.0f, 0.0f };

		inside ? ret = PhysMath::ClosestRectIntersectionFromInsidePoint(point, rect) : ret = PhysMath::ClosestRectIntersectionFromOutsidePoint(point, rect);

		return ret;
	}

	inline bool RayCast_Internal(PhysRay r1, PhysRay r2, PhysVec& ret)
	{
		PhysVec a = r1.start;
		PhysVec b = r1.end;

		PhysVec c = r2.start;
		PhysVec d = r2.end;

		PhysVec r = (b - a);
		PhysVec s = (d - c);

		float div = (r.x * s.y) - (r.y * s.x);
		float u = ((c.x - a.x) * r.y - (c.y - a.y) * r.x) / div;
		float t = ((c.x - a.x) * s.y - (c.y - a.y) * s.x) / div;

		ret = (a + (r * t));
		return (u >= 0 && u <= 1 && t >= 0 && t <= 1);
	}

	// Returns normal vec of the first intersecting plane (returned value is final pos of the vector, init is 0,0)
	// Works only for non-rotative rectangles
	inline bool RayCast(PhysRay ray, PhysRect rect, PhysVec& ret)
	{
		PhysRay rectPlanes[4] = { 
			{rect.Position(), rect.Position() + PhysVec(rect.w, 0) },
			{rect.Position() + PhysVec(rect.w, 0), rect.Position() + rect.Size() },
			{rect.Position() + rect.Size(), rect.Position() + PhysVec(0, rect.h) },
			{rect.Position() + PhysVec(0, rect.h), rect.Position() }
		};

		PhysVec intr;
		bool exist = false;
		int i = 0;

		for (i = 0; i < 4; ++i)
		{
			if (!RayCast_Internal(ray, rectPlanes[i], intr)) continue;

			exist = true;
			break;
		}

		if (!exist) return false;

		if (i == 0 || i == 2) ret = { 0.0f, 1.0f };
		else ret = { 1.0f, 0.0f };

		//if (intr.x == rect.x || intr.x == rect.x + rect.w) ret = {1.0f, 0.0f };
		//else if (intr.y == rect.y || intr.y == rect.y + rect.h) ret = { 0.0f, 1.0f };

		return true;

	}

	// Returns whether the ray intersects the rectangle
	// Works only for non-rotative rectangles
	inline bool RayCast(PhysRay ray, PhysRect rect)
	{
		PhysRay rectPlanes[4] = {
			{rect.Position(), rect.Position() + PhysVec(rect.w, 0) },
			{rect.Position() + PhysVec(rect.w, 0), rect.Position() + rect.Size() },
			{rect.Position() + rect.Size(), rect.Position() + PhysVec(0, rect.h) },
			{rect.Position() + PhysVec(0, rect.h), rect.Position() }
		};

		PhysVec intr;

		for (unsigned short int i = 0; i < 4; ++i)
		{
			if (RayCast_Internal(ray, rectPlanes[i], intr)) return true;
		}
		return false;
	}
};
