#pragma once

#include "Body.h"

class GasBody : public Body
{
public:

	GasBody(Rect rect, float mass, float heatRatio, float pressure, const float* pixelsToMeters);

	~GasBody() override;

	float GetDensity() const
	{
		return mass / rect.GetArea();
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

	float heatRatio = 0;
	float pressure = 0;
};
