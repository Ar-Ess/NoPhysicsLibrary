#pragma once

#include "Body.h"

class GasBody : public Body
{
public:

	GasBody(Rect rect, float mass, float heatRatio, float pressure, Point dragCoefficient, const float* pixelsToMeters);

	~GasBody() override;

	float GetDensity() const
	{
		//-TODO: available to return as pixels. look how liquidbody does it
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

	Point GetDragCoefficient() const
	{
		return dragCoefficient;
	}

private:

	float heatRatio = 0;
	float pressure = 0;
	Point dragCoefficient = {1.0f, 1.0f};
};
