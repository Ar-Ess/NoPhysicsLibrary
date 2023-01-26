#pragma once
#define PI 3.14159265359

#include "Point.h"
#include "Actions.h"

enum class EaseType
{
	LINEAR,
	QUADRATIC,
	CUBIC,
	QUARTIC,
	QUINTIC,
	SINOIDAL,
	EXPONENTIAL,
	CIRCULAR,
	BACK,
	ELASTIC,
	BOUNCE
};

enum class EaseMode
{
	IN_MODE,
	OUT_MODE,
	IN_OUT_MODE
};

class Easing
{
public:

	Easing(EaseType type, EaseMode mode);

	void Ease(Point* position, Point start, Point end, float duration, float dt);

	void OnEaseEnd(EaseType type, EaseMode mode);

private:

	// Actions

	void EndEase();

	void OnEndTypeChange(EaseType type, EaseMode mode);

	// Eases

	double LinearEase      (double t, double d);
	double QuadEaseIn      (double t, double d);
	double QuadEaseOut     (double t, double d);
	double QuadEaseInOut   (double t, double d);
	double CubicEaseIn     (double t, double d);
	double CubicEaseOut    (double t, double d);
	double CubicEaseInOut  (double t, double d);
	double QuartEaseIn     (double t, double d);
	double QuartEaseOut    (double t, double d);
	double QuartEaseInOut  (double t, double d);
	double QuintEaseIn     (double t, double d);
	double QuintEaseOut    (double t, double d);
	double QuintEaseInOut  (double t, double d);
	double SineEaseIn      (double t, double d);
	double SineEaseOut     (double t, double d);
	double SineEaseInOut   (double t, double d);
	double ExpoEaseIn      (double t, double d);
	double ExpoEaseOut     (double t, double d);
	double ExpoEaseInOut   (double t, double d);
	double CircEaseIn      (double t, double d);
	double CircEaseOut     (double t, double d);
	double CircEaseInOut   (double t, double d);
	double BackEaseIn      (double t, double d);
	double BackEaseOut     (double t, double d);
	double BackEaseInOut   (double t, double d);
	double ElasticEaseIn   (double t, double d);
	double ElasticEaseOut  (double t, double d);
	double ElasticEaseInOut(double t, double d);
	double BounceEaseIn    (double t, double d);
	double BounceEaseOut   (double t, double d);
	double BounceEaseInOut (double t, double d);

private:

	CAction<Easing, EaseType, EaseMode> onEnd;

	EaseType onEndType = EaseType::LINEAR;
	EaseMode onEndMode = EaseMode::IN_MODE;

	EaseType type = EaseType::LINEAR;
	EaseMode mode = EaseMode::IN_MODE;
	float timer = 0;
	bool active = true;
	const float s = 1.70158;
	const float r = 7.5625;

};