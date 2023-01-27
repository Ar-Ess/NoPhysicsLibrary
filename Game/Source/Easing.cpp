#include "Easing.h"
#include <math.h>

Easing::Easing(EaseType type, EaseMode mode, float duration)
{
	params = EasingParams(type, mode, duration, true);
	end = params;
	onEnd += std::bind(&Easing::OnEnd, this);
}

Point Easing::Ease(Point start, Point end, float dt)
{
	if (!params.active) return end;

	float val = 0;

	if (timer >= params.duration) timer = params.duration;

	switch (params.type)
	{
	case EaseType::LINEAR: 
	{
		val = LinearEase(timer, params.duration); break;
	}
	case EaseType::QUADRATIC:
	{
		switch (params.mode)
		{
		case EaseMode::IN_MODE: val = QuadEaseIn(timer, params.duration); break;
		case EaseMode::OUT_MODE: val = QuadEaseOut(timer, params.duration); break;
		case EaseMode::IN_OUT_MODE: val = QuadEaseInOut(timer, params.duration); break;
		}
		break;
	}
	case EaseType::CUBIC:
	{
		switch (params.mode)
		{
		case EaseMode::IN_MODE:     val = CubicEaseIn(timer, params.duration); break;
		case EaseMode::OUT_MODE:    val = CubicEaseOut(timer, params.duration); break;
		case EaseMode::IN_OUT_MODE: val = CubicEaseInOut(timer, params.duration); break;
		}
		break;
	}
	case EaseType::QUARTIC:
	{
		switch (params.mode)
		{
		case EaseMode::IN_MODE:     val = QuartEaseIn(timer, params.duration); break;
		case EaseMode::OUT_MODE:    val = QuartEaseOut(timer, params.duration); break;
		case EaseMode::IN_OUT_MODE: val = QuartEaseInOut(timer, params.duration); break;
		}
		break;
	}
	case EaseType::QUINTIC:
	{
		switch (params.mode)
		{
		case EaseMode::IN_MODE:     val = QuintEaseIn(timer, params.duration); break;
		case EaseMode::OUT_MODE:    val = QuintEaseOut(timer, params.duration); break;
		case EaseMode::IN_OUT_MODE: val = QuintEaseInOut(timer, params.duration); break;
		}
		break;
	}
	case EaseType::SINOIDAL:
	{
		switch (params.mode)
		{
		case EaseMode::IN_MODE:     val = SineEaseIn(timer, params.duration); break;
		case EaseMode::OUT_MODE:    val = SineEaseOut(timer, params.duration); break;
		case EaseMode::IN_OUT_MODE: val = SineEaseInOut(timer, params.duration); break;
		}
		break;
	}
	case EaseType::EXPONENTIAL:
	{
		switch (params.mode)
		{
		case EaseMode::IN_MODE:     val = ExpoEaseIn(timer, params.duration); break;
		case EaseMode::OUT_MODE:    val = ExpoEaseOut(timer, params.duration); break;
		case EaseMode::IN_OUT_MODE: val = ExpoEaseInOut(timer, params.duration); break;
		}
		break;
	}
	case EaseType::CIRCULAR:
	{
		switch (params.mode)
		{
		case EaseMode::IN_MODE:     val = CircEaseIn(timer, params.duration); break;
		case EaseMode::OUT_MODE:    val = CircEaseOut(timer, params.duration); break;
		case EaseMode::IN_OUT_MODE: val = CircEaseInOut(timer, params.duration); break;
		}
		break;
	}
	case EaseType::BACK:
	{
		switch (params.mode)
		{
		case EaseMode::IN_MODE:     val = BackEaseIn(timer, params.duration); break;
		case EaseMode::OUT_MODE:    val = BackEaseOut(timer, params.duration); break;
		case EaseMode::IN_OUT_MODE: val = BackEaseInOut(timer, params.duration); break;
		}
		break;
	}
	case EaseType::ELASTIC:
	{
		switch (params.mode)
		{
		case EaseMode::IN_MODE:     val = ElasticEaseIn(timer, params.duration); break;
		case EaseMode::OUT_MODE:    val = ElasticEaseOut(timer, params.duration); break;
		case EaseMode::IN_OUT_MODE: val = ElasticEaseInOut(timer, params.duration); break;
		}
		break;
	}
	case EaseType::BOUNCE:
	{
		switch (params.mode)
		{
		case EaseMode::IN_MODE:     val = BounceEaseIn(timer, params.duration); break;
		case EaseMode::OUT_MODE:    val = BounceEaseOut(timer, params.duration); break;
		case EaseMode::IN_OUT_MODE: val = BounceEaseInOut(timer, params.duration); break;
		}
		break;
	}
	}

	if (timer == params.duration) onEnd.Invoke();
	else timer += dt;

	return { start.x + (end.x - start.x) * val, start.y + (end.y - start.y) * val };
}

void Easing::OnEaseEnd(EaseType type, EaseMode mode)
{
	end.type = type;
	end.mode = mode;
}

void Easing::OnEaseEnd(bool active)
{
	if (!params.active) params.active = active;
	end.active = active;
}

void Easing::OnEaseEnd(float duration)
{
	end.duration = duration;
}

void Easing::OnEnd()
{
	params = end;
	if (params.active) timer = 0;
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