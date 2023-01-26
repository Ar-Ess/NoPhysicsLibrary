#include "Easing.h"
#include <math.h>



Easing::Easing(EaseType type, EaseMode mode)
{
	this->type = type;
	this->mode = mode;
}

void Easing::Ease(Point* position, Point start, Point end, float duration, float dt)
{
	float val = 0;

	if (timer > duration) EndEase();

	switch (type)
	{
	case EaseType::LINEAR: 
	{
		val = LinearEase(timer, duration); break;
	}
	case EaseType::QUADRATIC:
	{
		switch (mode)
		{
		case EaseMode::IN_MODE: val = QuadEaseIn(timer, duration); break;
		case EaseMode::OUT_MODE: val = QuadEaseOut(timer, duration); break;
		case EaseMode::IN_OUT_MODE: val = QuadEaseInOut(timer, duration); break;
		}
		break;
	}
	case EaseType::CUBIC:
	{
		switch (mode)
		{
		case EaseMode::IN_MODE:     val = CubicEaseIn(timer, duration); break;
		case EaseMode::OUT_MODE:    val = CubicEaseOut(timer, duration); break;
		case EaseMode::IN_OUT_MODE: val = CubicEaseInOut(timer, duration); break;
		}
		break;
	}
	case EaseType::QUARTIC:
	{
		switch (mode)
		{
		case EaseMode::IN_MODE:     val = QuartEaseIn(timer, duration); break;
		case EaseMode::OUT_MODE:    val = QuartEaseOut(timer, duration); break;
		case EaseMode::IN_OUT_MODE: val = QuartEaseInOut(timer, duration); break;
		}
		break;
	}
	case EaseType::QUINTIC:
	{
		switch (mode)
		{
		case EaseMode::IN_MODE:     val = QuintEaseIn(timer, duration); break;
		case EaseMode::OUT_MODE:    val = QuintEaseOut(timer, duration); break;
		case EaseMode::IN_OUT_MODE: val = QuintEaseInOut(timer, duration); break;
		}
		break;
	}
	case EaseType::SINOIDAL:
	{
		switch (mode)
		{
		case EaseMode::IN_MODE:     val = SineEaseIn(timer, duration); break;
		case EaseMode::OUT_MODE:    val = SineEaseOut(timer, duration); break;
		case EaseMode::IN_OUT_MODE: val = SineEaseInOut(timer, duration); break;
		}
		break;
	}
	case EaseType::EXPONENTIAL:
	{
		switch (mode)
		{
		case EaseMode::IN_MODE:     val = ExpoEaseIn(timer, duration); break;
		case EaseMode::OUT_MODE:    val = ExpoEaseOut(timer, duration); break;
		case EaseMode::IN_OUT_MODE: val = ExpoEaseInOut(timer, duration); break;
		}
		break;
	}
	case EaseType::CIRCULAR:
	{
		switch (mode)
		{
		case EaseMode::IN_MODE:     val = CircEaseIn(timer, duration); break;
		case EaseMode::OUT_MODE:    val = CircEaseOut(timer, duration); break;
		case EaseMode::IN_OUT_MODE: val = CircEaseInOut(timer, duration); break;
		}
		break;
	}
	case EaseType::BACK:
	{
		switch (mode)
		{
		case EaseMode::IN_MODE:     val = BackEaseIn(timer, duration); break;
		case EaseMode::OUT_MODE:    val = BackEaseOut(timer, duration); break;
		case EaseMode::IN_OUT_MODE: val = BackEaseInOut(timer, duration); break;
		}
		break;
	}
	case EaseType::ELASTIC:
	{
		switch (mode)
		{
		case EaseMode::IN_MODE:     val = ElasticEaseIn(timer, duration); break;
		case EaseMode::OUT_MODE:    val = ElasticEaseOut(timer, duration); break;
		case EaseMode::IN_OUT_MODE: val = ElasticEaseInOut(timer, duration); break;
		}
		break;
	}
	case EaseType::BOUNCE:
	{
		switch (mode)
		{
		case EaseMode::IN_MODE:     val = BounceEaseIn(timer, duration); break;
		case EaseMode::OUT_MODE:    val = BounceEaseOut(timer, duration); break;
		case EaseMode::IN_OUT_MODE: val = BounceEaseInOut(timer, duration); break;
		}
		break;
	}
	}

	position->x = start.x + (end.x - start.x) * val;
	position->y = start.y + (end.y - start.y) * val;

	timer += dt;
}

void Easing::OnEaseEnd(EaseType type, EaseMode mode)
{
	onEnd += &Easing::OnEndTypeChange;
	onEndType = type;
	onEndMode = mode;
}

void Easing::EndEase()
{
	timer = 0;
	onEnd.Invoke(onEndType, onEndMode, this);
}

void Easing::OnEndTypeChange(EaseType type, EaseMode mode)
{
	this->type = type;
	this->mode = mode;
	onEnd -= &Easing::OnEndTypeChange;
}

double Easing::LinearEase      (double t, double d)
{
	return (t / d);
}
double Easing::QuadEaseIn      (double t, double d)
{
	t /= d;
	return t * t;
}
double Easing::QuadEaseOut     (double t, double d)
{
	t /= d;
	return -1 * t * (t - 2);
}
double Easing::QuadEaseInOut   (double t, double d)
{
	if ((t /= d / 2) < 1) return ((1 / 2) * (t * t));

	return 1 - (-1 / 2 * (((--t) * (t - 2)) - 1));
}
double Easing::CubicEaseIn     (double t, double d)
{
	t /= d;
	return t * t * t;
}
double Easing::CubicEaseOut    (double t, double d)
{
	t /= d;
	t--;
	return (t * t * t + 1);
}
double Easing::CubicEaseInOut  (double t, double d)
{
	t /= d / 2;
	if (t < 1) return 1 / 2 * t * t * t;
	t -= 2;
	return 1 / 2 * (t * t * t + 2);
}
double Easing::QuartEaseIn     (double t, double d)
{
	t /= d;
	return t * t * t * t;
}
double Easing::QuartEaseOut    (double t, double d)
{
	t /= d;
	t--;
	return -1 * (t * t * t * t - 1);
}
double Easing::QuartEaseInOut  (double t, double d)
{
	t /= d / 2;
	if (t < 1) return 1 / 2 * t * t * t * t;
	t -= 2;
	return -1 / 2 * (t * t * t * t - 2);
}
double Easing::QuintEaseIn     (double t, double d)
{
	t /= d;
	return t * t * t * t * t;
}
double Easing::QuintEaseOut    (double t, double d)
{
	t /= d;
	t--;
	return (t * t * t * t * t + 1);
}
double Easing::QuintEaseInOut  (double t, double d)
{
	t /= d / 2;
	if (t < 1) return 1 / 2 * t * t * t * t * t;
	t -= 2;
	return 1 / 2 * (t * t * t * t * t + 2);
}
double Easing::SineEaseIn      (double t, double d) 
{
	return -1 * cos(t / d * (PI / 2)) + 1;
}
double Easing::SineEaseOut     (double t, double d) 
{
	return 1 * sin(t / d * (PI / 2));
}
double Easing::SineEaseInOut   (double t, double d) 
{
	return -1 / 2 * (cos(PI * t / d) - 1);
}
double Easing::ExpoEaseIn      (double t, double d) 
{
	return pow(2, 10 * float(t / d - 1));
}
double Easing::ExpoEaseOut     (double t, double d) 
{
	return (-pow(2, -10 * t / d) + 1);
}
double Easing::ExpoEaseInOut   (double t, double d) 
{
	t /= d / 2;
	if (t < 1) return 1 / 2 * pow(2, 10 * float(t - 1));
	t--;
	return 1 / 2 * (-pow(2, float(-10 * t)) + 2);
}
double Easing::CircEaseIn      (double t, double d) 
{
	t /= d;
	return -1 * (sqrt(1 - float(t * t)) - 1);
}
double Easing::CircEaseOut     (double t, double d) 
{
	t /= d;
	t--;
	return sqrt(1 - float(t * t));
}
double Easing::CircEaseInOut   (double t, double d) 
{
	t /= d / 2;
	if (t < 1) return -1 / 2 * (sqrt(1 - float(t * t)) - 1);
	t -= 2;
	return 1 / 2 * (sqrt(1 - float(t * t)) + 1);
}
double Easing::BackEaseIn      (double t, double d) 
{
	return (t /= d) * t * ((s + 1) * t - s);
}
double Easing::BackEaseOut     (double t, double d) 
{
	return ((t = t / d - 1) * t * ((s + 1) * t + s) + 1);
}
double Easing::BackEaseInOut   (double t, double d) 
{
	float sval = s;
	if ((t /= d / 2) < 1) return 1 / 2 * (t * t * (((sval *= (1.525)) + 1) * t - s));
	return 1 / 2 * ((t -= 2) * t * (((sval *= (1.525)) + 1) * t + s) + 2);
}
double Easing::ElasticEaseIn   (double t, double d) 
{
	if (t == 0) return 0;

	if ((t /= d) == 1) return 1;

	float p = d * .3f;
	float a = 1;
	float s = p / 4;
	return -(a * pow(2, 10 * float(t -= 1)) * sin((float(t * d) - s) * (2 * PI) / p));
}
double Easing::ElasticEaseOut  (double t, double d) 
{
	if (t == 0) return 0;

	if ((t /= d) == 1) return 1;

	float p = d * .3f;
	float a = 1;
	float s = p / 4;
	return a * pow(2, float(-10 * t)) * sin((float(t * d) - s) * (2 * PI) / p) + 1;
}
double Easing::ElasticEaseInOut(double t, double d) 
{
	if (t == 0) return 0;

	if ((t /= d / 2) == 2) return 1;

	float p = d * (.3f * 1.5f);
	float a = 1;
	float s = p / 4;

	if (t < 1) return -.5f * (a * pow(2, 10 * float(t -= 1)) * sin((float(t * d) - s) * (2 * PI) / p));

	return a * pow(2, -10 * float(t -= 1)) * sin((float(t * d) - s) * (2 * PI) / p) * .5f + 1;
}
double Easing::BounceEaseIn    (double t, double d) 
{
	return 1 - BounceEaseOut(d - t, d);
}
double Easing::BounceEaseOut   (double t, double d) 
{
	if ((t /= d) < (1 / 2.75)) return (r * t * t);

	if (t < (2 / 2.75)) return (r * (t -= float(1.5 / 2.75)) * t + .75);

	if (t < (2.5 / 2.75)) return (r * (t -= float(2.25 / 2.75)) * t + .9375);
	
	return (r * (t -= float(2.625 / 2.75)) * t + .984375);
}
double Easing::BounceEaseInOut (double t, double d) 
{
	if (t < d / 2) return BounceEaseIn(t * 2, d) * .5;

	return BounceEaseOut(t * 2 - d, d) * .5 + .5;
}