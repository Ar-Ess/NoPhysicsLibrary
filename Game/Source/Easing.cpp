#include "Easing.h"
#include "MathUtils.h"

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
	t /= d;
	return t < 0.5 ? 2 * t * t : 1 - (double)MathUtils::Pow(-2 * t + 2, 2) / 2;
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
	t /= d;
	return t < 0.5 ? 4 * t * t * t : 1 - (double)MathUtils::Pow(-2 * t + 2, 3) / 2;
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
	t /= d;
	return t < 0.5 ? 8 * t * t * t * t : 1 - (double)MathUtils::Pow(-2 * t + 2, 4) / 2;
}
double Easing::QuintEaseIn     (double t, double d)
{
	t /= d;
	return  (double)MathUtils::Pow(t, 5);
}
double Easing::QuintEaseOut    (double t, double d)
{
	t /= d;
	return 1 - (double)MathUtils::Pow(1 - t, 5);
}
double Easing::QuintEaseInOut  (double t, double d)
{
	t /= d;
	return t < 0.5 ? 16 * t * t * t * t * t : 1 - MathUtils::Pow(-2 * t + 2, 5) / 2;
}
double Easing::SineEaseIn      (double t, double d) 
{
	return -1 * (double)MathUtils::Cos(t / d * (PI / 2)) + 1;
}
double Easing::SineEaseOut     (double t, double d) 
{
	return 1 * (double)MathUtils::Sin(t / d * (PI / 2));
}
double Easing::SineEaseInOut   (double t, double d) 
{
	if (t < d / 2) return SineEaseIn(t * 2, d) * .5;
	return SineEaseOut(t * 2 - d, d) * .5 + .5;
}
double Easing::ExpoEaseIn      (double t, double d) 
{
	return MathUtils::Pow(2, 10 * float(t / d - 1));
}
double Easing::ExpoEaseOut     (double t, double d) 
{
	return (-MathUtils::Pow(2, -10 * t / d) + 1);
}
double Easing::ExpoEaseInOut   (double t, double d) 
{
	t /= d;
	return t == 0
		? 0
		: t == 1
		? 1
		: t < 0.5 ? MathUtils::Pow(2, 20 * t - 10) / 2
		: (2 - MathUtils::Pow(2, -20 * t + 10)) / 2;
}
double Easing::CircEaseIn      (double t, double d) 
{
	t /= d;
	return -1 * ((double)MathUtils::Sqrt(1 - float(t * t)) - 1);
}
double Easing::CircEaseOut     (double t, double d) 
{
	t /= d;
	t--;
	return MathUtils::Sqrt(1 - float(t * t));
}
double Easing::CircEaseInOut   (double t, double d) 
{
	t /= d;
	return t < 0.5
		? (1 - MathUtils::Sqrt(1 - MathUtils::Pow(2 * t, 2))) / 2
		: (MathUtils::Sqrt(1 - MathUtils::Pow(-2 * t + 2, 2)) + 1) / 2;
}
double Easing::BackEaseIn      (double t, double d) 
{
	t /= d;
	return c3 * t * t * t - c1 * t * t;
}
double Easing::BackEaseOut     (double t, double d) 
{
	t /= d;
	return 1 + c3 * MathUtils::Pow(t - 1, 3) + c1 * MathUtils::Pow(t - 1, 2);
}
double Easing::BackEaseInOut   (double t, double d) 
{
	t /= d;
	return t < 0.5
		? (MathUtils::Pow(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) / 2
		: (MathUtils::Pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2) / 2;
}
double Easing::ElasticEaseIn   (double t, double d) 
{
	if (t == 0) return 0;

	if ((t /= d) == 1) return 1;

	double p = d * .3f;
	double a = 1;
	double s = p / 4;
	return -(a * MathUtils::Pow(2, 10 * float(t -= 1)) * MathUtils::Sin((double(t * d) - s) * (2 * PI) / p));
}
double Easing::ElasticEaseOut  (double t, double d) 
{
	if (t == 0) return 0;

	if ((t /= d) == 1) return 1;

	double p = d * .3f;
	double a = 1;
	double s = p / 4;
	return a * MathUtils::Pow(2, float(-10 * t)) * MathUtils::Sin((double(t * d) - s) * (2 * PI) / p) + 1;
}
double Easing::ElasticEaseInOut(double t, double d) 
{
	if (t == 0) return 0;

	if ((t /= d / 2) == 2) return 1;

	double p = d * (.3 * 1.5);
	double a = 1;
	double s = p / 4;

	if (t < 1) return -.5f * (a * MathUtils::Pow(2, 10 * float(t -= 1)) * MathUtils::Sin((double(t * d) - s) * (2 * PI) / p));

	return a * MathUtils::Pow(2, -10 * float(t -= 1)) * MathUtils::Sin((double(t * d) - s) * (2 * PI) / p) * .5f + 1;
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