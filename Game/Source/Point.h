#pragma once

#include <math.h>
#include "PhysVec.h"

struct Point
{
	Point() {}
	Point(float x, float y) { this->x = x; this->y = y; }
	Point(double x, double y) { this->x = float(x); this->y = float(y); }

	Point(float a) { x = a; y = a; }
	Point(double a) { x = float(a); y = float(a); }

	Point(PhysVec vec)
	{
		this->x = vec.x;
		this->y = vec.y;
	}

	float x = 0, y = 0;

	float Distance(Point to)
	{
		float x = (float)pow(to.x - double(this->x), 2);
		float y = (float)pow(to.y - double(this->y), 2);

		return (float)sqrt(double(x) + y);
	}
	float Distance(float x, float y)
	{
		float xx = (float)pow(x - double(this->x), 2);
		float yy = (float)pow(y - double(this->y), 2);

		return (float)sqrt(double(xx) + yy);
	}
	float Distance(double x, double y)
	{
		float xx = (float)pow(x - double(this->x), 2);
		float yy = (float)pow(y - double(this->y), 2);

		return (float)sqrt(double(xx) + yy);
	}
	float Distance(int x, int y)
	{
		float xx = (float)pow(double(x) - double(this->x), 2);
		float yy = (float)pow(double(y) - double(this->y), 2);

		return (float)sqrt(double(xx) + yy);
	}

	float Module()
	{
		return (float)sqrt(pow(double(x), 2) + pow(double(y), 2));
	}

	void Zero()
	{
		this->x = 0;
		this->y = 0;
	}

	bool IsZero()
	{
		return (x == 0 && y == 0);
	}

	// Negates the point internally, modifying its values
	void Negate()
	{
		this->x *= -1;
		this->y *= -1;
	}
	
	// Returns the point negated. Does not modify this point internally.
	Point Negative()
	{
		return Point(this->x * -1,  this->y * -1);
	}

	// Returns is one or the other is negative. Set both to true to check if both are negative
	bool IsNegative(bool both = false)
	{
		if (both) return x < 0 && y < 0;

		return x < 0 || y < 0;
	}

	void Set(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	void Abs()
	{
		if (this->x < 0) this->x *= -1;
		if (this->y < 0) this->y *= -1;
	}

	// Function: It applies the number inputed to the actual Point value and the result is the output.
	// It does not modify the value stored in the class
	inline Point Apply(Point sum) const
	{
		return Point{this->x + sum.x, this->y + sum.y};
	}
	inline Point Apply(float x, float y) const
	{
		return Point{ this->x + x, this->y + y };
	}
	inline Point Apply(double x, double y) const
	{
		return Point{ this->x + x, this->y + y };
	}

	inline Point Multiply(Point multi) const
	{
		return Point{ this->x * multi.x, this->y * multi.y };
	}
	inline Point Multiply(float x, float y) const
	{
		return Point{ this->x * x, this->y * y };
	}
	inline Point Multiply(double x, double y) const
	{
		return Point{ this->x * x, this->y * y };
	}

	inline float GetMax()
	{
		return (float)fmax(x, y);
	}

	inline float GetMaxAbs()
	{
		return (float)fmax(fabsf(x), fabsf(y));
	}

	float operator[](int i) 
	{
		float ret = 0;

		//--Cond--  --True--  ------------False------------
		//   ||        ||     --Cond--  --True--  --False--
		//   \/        \/        \/        \/         \/
		(i == 0) ? ret = x : ((i == 1) ? ret = y : ret = 0);

		return ret;
	}

	// Conditional operators
	inline bool operator==(float i) { return (x == i || y == i); }
	inline bool operator!=(float i) { return !(x == i && y == i); }
	inline bool operator< (float i) { return (x < i&& y < i); }
	inline bool operator<=(float i) { return (x <= i && y <= i); }
	inline bool operator> (float i) { return (x > i && y > i); }
	inline bool operator>=(float i) { return (x >= i && y >= i); }

	inline bool operator==(Point b) { return (x == b.x && y == b.y); }
	inline bool operator!=(Point b) { return !(x == b.x && y == b.y); }
	inline bool operator< (Point b) { return (x < b.x&& y < b.y); }
	inline bool operator<=(Point b) { return (x <= b.x && y <= b.y); }
	inline bool operator> (Point b) { return (x > b.x && y > b.y); }
	inline bool operator>=(Point b) { return (x >= b.x && y >= b.y); }

	// Modification operators
	inline void operator+=(Point i) { x += i.x; y += i.y; }
	inline void operator-=(Point i) { x -= i.x; y -= i.y; }
	inline void operator*=(Point i) { x *= i.x; y *= i.y; }
	inline void operator/=(Point i) { if (i == 0) return; x /= i.x; y /= i.y; }

	inline void operator+=(float i) { x += i; y += i; }
	inline void operator-=(float i) { x -= i; y -= i; }
	inline void operator*=(float i) { x *= i; y *= i; }
	inline void operator/=(float i) { if (i == 0) return; x /= i; y /= i; }

	inline Point operator+(Point i) { return Point{ x + i.x, y + i.y }; }
	inline Point operator+(float i) { return Point{ x + i, y + i }; }

	inline Point operator-(Point i) { return Point{ x - i.x, y - i.y }; }
	inline Point operator-(float i) { return Point{ x - i, y - i }; }

	inline Point operator*(Point i) { return Point{ x * i.x, y * i.y }; }
	inline Point operator*(float i) { return Point{ x * i, y * i }; }

	inline Point operator/(float div) { return Point{ x / div, y / div }; }
	inline Point operator/(Point i) { return Point{ x / i.x, y / i.y }; }

	// Asign operators
	inline Point& operator = (const Point& u)
	{
		x = u.x; y = u.y; return *this;
	}

};