#pragma once

struct PhysVec
{
	PhysVec();
	PhysVec(float x, float y);
	PhysVec(int x, int y);
	PhysVec(int x, float y);
	PhysVec(float x, int y);

	~PhysVec() {}

	PhysVec operator*(PhysVec value) const;
	PhysVec operator*(float value) const;
	PhysVec operator*(int value) const;
	PhysVec operator+(PhysVec value) const;
	PhysVec operator+(float value) const;
	PhysVec operator+(int value) const;
	PhysVec operator-(PhysVec value) const;
	PhysVec operator-(float value) const;
	PhysVec operator-(int value) const;
	PhysVec operator/(PhysVec value) const;
	PhysVec operator/(float value) const;
	PhysVec operator/(int value) const;
	void operator=(float value);
	void operator=(int value);
	void operator*=(PhysVec value);
	void operator*=(float value);
	void operator*=(int value);
	void operator+=(PhysVec value);
	void operator+=(float value);
	void operator+=(int value);
	void operator-=(PhysVec value);
	void operator-=(float value);
	void operator-=(int value);

	void Clear();

	const bool IsZero() const;

	float x = 0, y = 0;
	float module = 0.0f;

};

typedef PhysVec Force;
typedef PhysVec Momentum;
