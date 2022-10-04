#pragma once

#include "Body.h"

class GasBody : public Body
{
public:

	GasBody(Rect rect, float mass, float density, float heatRatio, float pressure, const float* pixelsToMeters);

	~GasBody() override;

	float GetDensity() const
	{
		return density;
	}

	float GetHeatRatio() const
	{
		return heatRatio;
	}

	float GetPressure() const
	{
		return pressure;
	}

private:

	float density = 0;
	float heatRatio = 0;
	float pressure = 0;
};
