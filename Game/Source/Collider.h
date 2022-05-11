#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "Rect.h"
#include "Circle.h"

class Collision
{
public:
	Collision() {}

	double Distance(int x1, int y1, int x2, int y2)
	{
		return sqrt(pow(double(x2 - x1), 2) + pow(double(y2 - y1), 2));
	}

	bool CheckCollision(Rect r1, Rect r2) const
	{
		if ((r1.x > r2.x + r2.w) || (r1.x + r1.w < r2.x) || (r1.y > r2.y + r2.h) || (r1.y + r1.h < r2.y)) return false;
		return true;
	}

	bool CheckCollision(CircleCollider& a, Rect& b)
	{
		//Closest point on collision box
		int cX, cY;

		//Find closest x offset
		if (a.x < b.x)
		{
			cX = b.x;
		}
		else if (a.x > b.x + b.w)
		{
			cX = b.x + b.w;
		}
		else
		{
			cX = a.x;
		}
		//Find closest y offset
		if (a.y < b.y)
		{
			cY = b.y;
		}
		else if (a.y > b.y + b.h)
		{
			cY = b.y + b.h;
		}
		else
		{
			cY = a.y;
		}

		//If the closest point is inside the circle
		if (int dist = Distance(a.x, a.y, cX, cY) < a.radius)
		{
			//This box and the circle have collided
			if (dist == 0.0f)
				return false;
			return true;
		}
		//If the shapes have not collided
		return false;
	}

	bool CheckCollision(Rect& b, CircleCollider& a)
	{
		//Closest point on collision box
		int cX, cY;

		//Find closest x offset
		if (a.x < b.x)
		{
			cX = b.x;
		}
		else if (a.x > b.x + b.w)
		{
			cX = b.x + b.w;
		}
		else
		{
			cX = a.x;
		}
		//Find closest y offset
		if (a.y < b.y)
		{
			cY = b.y;
		}
		else if (a.y > b.y + b.h)
		{
			cY = b.y + b.h;
		}
		else
		{
			cY = a.y;
		}

		//If the closest point is inside the circle
		if (int dist = Distance(a.x, a.y, cX, cY) < a.radius)
		{
			//This box and the circle have collided
			if (dist == 0.0f)
				return false;
			return true;
		}
		//If the shapes have not collided
		return false;
	}

	bool CheckCollision(CircleCollider& A, CircleCollider& B)
	{
		//If the distance between the centers of the circles is less than the sum of their radii
		if (Distance(A.x, A.y, B.x, B.y) < (A.radius + B.radius))
		{
			//The circles have collided
			return true;
		}

		//If not
		return false;
	}
};

#endif
