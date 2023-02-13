#pragma once

struct PhysVec
{
	PhysVec();
	PhysVec(float x, float y);
	PhysVec(int x, int y);

	~PhysVec() {}

	PhysVec operator*(PhysVec value);
	PhysVec operator*(float value);
	PhysVec operator*(int value);
	PhysVec operator+(PhysVec value);
	PhysVec operator+(float value);
	PhysVec operator+(int value);

	void Clear();

	float x = 0, y = 0;
	float module = 0.0f;

};

typedef PhysVec Force;
typedef PhysVec Momentum;
