#pragma once
#define PI 3.14159265359

#include "Point.h"
#include "Action.h"

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
private:

	struct EasingParams
	{
		EasingParams() {}

		EasingParams(EaseType type, EaseMode mode, float duration, bool active)
		{
			this->type = type;
			this->mode = mode;
			this->duration = duration;
			this->active = active;
		}

		EaseType type = EaseType::LINEAR;
		EaseMode mode = EaseMode::IN_MODE;
		float duration = 0.0f;
		bool active = true;
	};

public:

	Easing(EaseType type, EaseMode mode, float duration);

	// Returns the offset to the start position regarding the elapsed time
	Point Ease(Point start, Point end, float dt);

	void OnEaseEnd(EaseType type, EaseMode mode);
	void OnEaseEnd(bool active);
	void OnEaseEnd(float duration);

private:

	// Actions
	void OnEnd();

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

	EasingParams end;
	Action<> onEnd;

	EasingParams params;
	float timer = 0;
	const float s = 1.70158;
	const float r = 7.5625;

};