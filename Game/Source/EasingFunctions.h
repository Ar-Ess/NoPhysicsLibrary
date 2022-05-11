#pragma once
#define PI 3.14159265359

#include "Point.h"

enum class EasingType
{
	LINEAR,
	QUADRATIC_IN,
	QUADRATIC_OUT,
	QUADRATIC_IN_OUT,
	CUBIC_IN,
	CUBIC_OUT,
	CUBIC_IN_OUT,
	QUARTIC_IN,
	QUARTIC_OUT,
	QUARTIC_IN_OUT,
	QUINTIC_IN,
	QUINTIC_OUT,
	QUINTIC_IN_OUT,
	SINOIDAL_IN,
	SINOIDAL_OUT,
	SINOIDAL_IN_OUT,
	EXPONENTIAL_IN,
	EXPONENTIAL_OUT,
	EXPONENTIAL_IN_OUT,
	CIRCULAR_IN,
	CIRCULAR_OUT,
	CIRCULAR_IN_OUT,
	BACK_IN,
	BACK_OUT,
	BACK_IN_OUT,
	ELASTIC_IN,
	ELASTIC_OUT,
	ELASTIC_IN_OUT,
	BOUNCE_IN,
	BOUNCE_OUT,
	BOUNCE_IN_OUT
};

class EasingFunctions 
{
public:
	Point Ease(EasingType easeType, Point finalPosition, Point initialPosition, bool repeat, int totalIterX, int totalIterY);
	void ResetIterations();

private:
	static float linearEaseNull(float t, float b, float c, float d);

	static float quadEaseIn(float t, float b, float c, float d);
	static float quadEaseOut(float t, float b, float c, float d);
	static float quadEaseInOut(float t, float b, float c, float d);

	static float cubicEaseIn(float t, float b, float c, float d);
	static float cubicEaseOut(float t, float b, float c, float d);
	static float cubicEaseInOut(float t, float b, float c, float d);

	static float quartEaseIn(float t, float b, float c, float d);
	static float quartEaseOut(float t, float b, float c, float d);
	static float quartEaseInOut(float t, float b, float c, float d);

	static float quintEaseIn(float t, float b, float c, float d);
	static float quintEaseOut(float t, float b, float c, float d);
	static float quintEaseInOut(float t, float b, float c, float d);

	static float sineEaseIn(float t, float b, float c, float d);
	static float sineEaseOut(float t, float b, float c, float d);
	static float sineEaseInOut(float t, float b, float c, float d);

	static float expoEaseIn(float t, float b, float c, float d);
	static float expoEaseOut(float t, float b, float c, float d);
	static float expoEaseInOut(float t, float b, float c, float d);

	static float circEaseIn(float t, float b, float c, float d);
	static float circEaseOut(float t, float b, float c, float d);
	static float circEaseInOut(float t, float b, float c, float d);

	static float backEaseIn(float t, float b, float c, float d);
	static float backEaseOut(float t, float b, float c, float d);
	static float backEaseInOut(float t, float b, float c, float d);

	static float elasticEaseIn(float t, float b, float c, float d);
	static float elasticEaseOut(float t, float b, float c, float d);
	static float elasticEaseInOut(float t, float b, float c, float d);

	static float bounceEaseIn(float t, float b, float c, float d);
	static float bounceEaseOut(float t, float b, float c, float d);
	static float bounceEaseInOut(float t, float b, float c, float d);

private:
	float t, b, c, d;
	int iterations = 0;
};

//IMPLEMENTATION
//fPoint ease = easing.Ease(EasingType::QUINTIC_IN_OUT, { 300, 300 }, { 0, 0 }, false, 100, 100);
//testButton->Update(0.0f);
//testButton->bounds.x -= ease.x;
//testButton->bounds.y -= ease.y;
//testButton->Draw();
//testButton->bounds.x += ease.x;
//testButton->bounds.y += ease.y;