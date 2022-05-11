#include "EasingFunctions.h"
#include <cmath>

//

Point EasingFunctions::Ease(EasingType eType, Point posA, Point posB, bool repeat, int totalIterX, int totalIterY)
{

	float valueX = 0.0f;
	float valueY = 0.0f;

	switch (eType)
	{
	case EasingType::LINEAR:
		valueX = linearEaseNull(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = linearEaseNull(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::QUADRATIC_IN:
		valueX = quadEaseIn(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = quadEaseIn(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::QUADRATIC_OUT:
		valueX = quadEaseOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = quadEaseOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::QUADRATIC_IN_OUT:
		valueX = quadEaseInOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = quadEaseInOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::CUBIC_IN:
		valueX = cubicEaseIn(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = cubicEaseIn(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::CUBIC_OUT:
		valueX = cubicEaseOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = cubicEaseOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::CUBIC_IN_OUT:
		valueX = cubicEaseInOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = cubicEaseInOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::QUARTIC_IN:
		valueX = quartEaseIn(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = quartEaseIn(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::QUARTIC_OUT:
		valueX = quartEaseOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = quartEaseOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::QUARTIC_IN_OUT:
		valueX = quartEaseInOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = quartEaseInOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::QUINTIC_IN:
		valueX = quintEaseIn(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = quintEaseIn(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::QUINTIC_OUT:
		valueX = quintEaseOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = quintEaseOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::QUINTIC_IN_OUT:
		valueX = quintEaseInOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = quintEaseInOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::SINOIDAL_IN:
		valueX = sineEaseIn(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = sineEaseIn(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::SINOIDAL_OUT:
		valueX = sineEaseOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = sineEaseOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::SINOIDAL_IN_OUT:
		valueX = sineEaseInOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = sineEaseInOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::EXPONENTIAL_IN:
		valueX = expoEaseIn(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = expoEaseIn(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::EXPONENTIAL_OUT:
		valueX = expoEaseOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = expoEaseOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::EXPONENTIAL_IN_OUT:
		valueX = expoEaseInOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = expoEaseInOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::CIRCULAR_IN:
		valueX = circEaseIn(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = circEaseIn(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::CIRCULAR_OUT:
		valueX = circEaseOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = circEaseOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::CIRCULAR_IN_OUT:
		valueX = circEaseInOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = circEaseInOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::BACK_IN:
		valueX = backEaseIn(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = backEaseIn(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::BACK_OUT:
		valueX = backEaseOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = backEaseOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::BACK_IN_OUT:
		valueX = backEaseInOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = backEaseInOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::ELASTIC_IN:
		valueX = elasticEaseIn(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = elasticEaseIn(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::ELASTIC_OUT:
		valueX = elasticEaseOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = elasticEaseOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::ELASTIC_IN_OUT:
		valueX = elasticEaseInOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = elasticEaseInOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::BOUNCE_IN:
		valueX = bounceEaseIn(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = bounceEaseIn(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::BOUNCE_OUT:
		valueX = bounceEaseOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = bounceEaseOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;

	case EasingType::BOUNCE_IN_OUT:
		valueX = bounceEaseInOut(iterations, posA.x, posB.x - posA.x, totalIterX);
		valueY = bounceEaseInOut(iterations, posA.y, posB.y - posA.y, totalIterY);
		break;
	}

	int totalIter = 0;
	if (totalIterX > totalIterY) totalIter = totalIterX;
	else totalIter = totalIterY;

	if (iterations < totalIter) 
	{
		iterations++;
	}
	else 
	{
		if (repeat)	iterations = 0;
	}

	if (iterations >= totalIterX) valueX = (float)posB.x;
	if (iterations >= totalIterY) valueY = (float)posB.y;
	if (totalIterX == 0) valueX = 0.0f;
	if (totalIterY == 0) valueY = 0.0f;

	return { valueX, valueY };
}

void EasingFunctions::ResetIterations()
{
	iterations = 0;
}

float EasingFunctions::linearEaseNull(float t, float b, float c, float d)
{
	return c * t / d + b;
}


//

float EasingFunctions::quadEaseIn(float t, float b, float c, float d)
{
	t /= d;
	return c * t * t + b;
}

float EasingFunctions::quadEaseOut(float t, float b, float c, float d)
{
	t /= d;
	return -c * t * (t - 2) + b;
}

float EasingFunctions::quadEaseInOut(float t, float b, float c, float d)
{
	t /= d / 2;
	if (t < 1) {
		return c / 2 * t * t + b;
	}
	t--;
	return -c / 2 * (t * (t - 2) - 1) + b;
}


//

float EasingFunctions::cubicEaseIn(float t, float b, float c, float d)
{
	t /= d;
	return c * t * t * t + b;
}

float EasingFunctions::cubicEaseOut(float t, float b, float c, float d)
{
	t /= d;
	t--;
	return c * (t * t * t + 1) + b;
}

float EasingFunctions::cubicEaseInOut(float t, float b, float c, float d)
{
	t /= d / 2;
	if (t < 1) {
		return c / 2 * t * t * t + b;
	}
	t -= 2;
	return c / 2 * (t * t * t + 2) + b;
}


//

float EasingFunctions::quartEaseIn(float t, float b, float c, float d)
{
	t /= d;
	return c * t * t * t * t + b;
}

float EasingFunctions::quartEaseOut(float t, float b, float c, float d)
{
	t /= d;
	t--;
	return -c * (t * t * t * t - 1) + b;
}

float EasingFunctions::quartEaseInOut(float t, float b, float c, float d)
{
	t /= d / 2;
	if (t < 1) return c / 2 * t * t * t * t + b;
	t -= 2;
	return -c / 2 * (t * t * t * t - 2) + b;
}


//

float EasingFunctions::quintEaseIn(float t, float b, float c, float d)
{
	t /= d;
	return c * t * t * t * t * t + b;
}

float EasingFunctions::quintEaseOut(float t, float b, float c, float d)
{
	t /= d;
	t--;
	return c * (t * t * t * t * t + 1) + b;
}

float EasingFunctions::quintEaseInOut(float t, float b, float c, float d)
{
	t /= d / 2;
	if (t < 1) return c / 2 * t * t * t * t * t + b;
	t -= 2;
	return c / 2 * (t * t * t * t * t + 2) + b;
}


//

float EasingFunctions::sineEaseIn(float t, float b, float c, float d) {
	return -c * cos(t / d * (PI / 2)) + c + b;
}

float EasingFunctions::sineEaseOut(float t, float b, float c, float d) {
	return c * sin(t / d * (PI / 2)) + b;
}

float EasingFunctions::sineEaseInOut(float t, float b, float c, float d) {
	return -c / 2 * (cos(PI * t / d) - 1) + b;
}


//

float EasingFunctions::expoEaseIn(float t, float b, float c, float d) {
	return c * pow(2, 10 * (t / d - 1)) + b;
}

float EasingFunctions::expoEaseOut(float t, float b, float c, float d) {
	return c * (-pow(2, -10 * t / d) + 1) + b;
}

float EasingFunctions::expoEaseInOut(float t, float b, float c, float d) {
	t /= d / 2;
	if (t < 1) return c / 2 * pow(2, 10 * (t - 1)) + b;
	t--;
	return c / 2 * (-pow(2, -10 * t) + 2) + b;
}


//

float EasingFunctions::circEaseIn(float t, float b, float c, float d) {
	t /= d;
	return -c * (sqrt(1 - t * t) - 1) + b;
}

float EasingFunctions::circEaseOut(float t, float b, float c, float d) {
	t /= d;
	t--;
	return c * sqrt(1 - t * t) + b;
}

float EasingFunctions::circEaseInOut(float t, float b, float c, float d) {
	t /= d / 2;
	if (t < 1) return -c / 2 * (sqrt(1 - t * t) - 1) + b;
	t -= 2;
	return c / 2 * (sqrt(1 - t * t) + 1) + b;
}


//

float EasingFunctions::backEaseIn(float t, float b, float c, float d) {
	float s = 1.70158;
	return c * (t /= d) * t * ((s + 1) * t - s) + b;
}

float EasingFunctions::backEaseOut(float t, float b, float c, float d) {
	float s = 1.70158;
	return c * ((t = t / d - 1) * t * ((s + 1) * t + s) + 1) + b;
}

float EasingFunctions::backEaseInOut(float t, float b, float c, float d) {
	float s = 1.70158f;
	if ((t /= d / 2) < 1) return c / 2 * (t * t * (((s *= (1.525)) + 1) * t - s)) + b;
	return c / 2 * ((t -= 2) * t * (((s *= (1.525)) + 1) * t + s) + 2) + b;
}


//

float EasingFunctions::elasticEaseIn(float t, float b, float c, float d) {
	if (t == 0)
		return b;
	if ((t /= d) == 1)
		return b + c;
	float p = d * .3f;
	float a = c;
	float s = p / 4;
	return -(a * pow(2, 10 * (t -= 1)) * sin((t * d - s) * (2 * PI) / p)) + b;
}

float EasingFunctions::elasticEaseOut(float t, float b, float c, float d) {
	if (t == 0) return b;
	if ((t /= d) == 1) return b + c;
	float p = d * .3f;
	float a = c;
	float s = p / 4;
	return (a * pow(2, -10 * t) * sin((t * d - s) * (2 * PI) / p) + c + b);
}

float EasingFunctions::elasticEaseInOut(float t, float b, float c, float d) {
	if (t == 0)
		return b;
	if ((t /= d / 2) == 2)
		return b + c;
	float p = d * (.3f * 1.5f);
	float a = c;
	float s = p / 4;
	if (t < 1)
		return -.5f * (a * pow(2, 10 * (t -= 1)) * sin((t * d - s) * (2 * PI) / p)) + b;
	return a * pow(2, -10 * (t -= 1)) * sin((t * d - s) * (2 * PI) / p) * .5f + c + b;
}


//

float EasingFunctions::bounceEaseIn(float t, float b, float c, float d) {
	return c - EasingFunctions::bounceEaseOut(d - t, 0, c, d) + b;
}

float EasingFunctions::bounceEaseOut(float t, float b, float c, float d) {
	if ((t /= d) < (1 / 2.75)) {
		return c * (7.5625 * t * t) + b;
	}
	else if (t < (2 / 2.75)) {
		return c * (7.5625 * (t -= (1.5 / 2.75)) * t + .75) + b;
	}
	else if (t < (2.5 / 2.75)) {
		return c * (7.5625 * (t -= (2.25 / 2.75)) * t + .9375) + b;
	}
	else {
		return c * (7.5625 * (t -= (2.625 / 2.75)) * t + .984375) + b;
	}
}

float EasingFunctions::bounceEaseInOut(float t, float b, float c, float d) {
	if (t < d / 2) return EasingFunctions::bounceEaseIn(t * 2, 0, c, d) * .5 + b;
	return EasingFunctions::bounceEaseOut(t * 2 - d, 0, c, d) * .5 + c * .5 + b;
}