#pragma once

#include "Body.h"

class GasBody : public Body
{
public:

	GasBody(Rect rect, float mass, float heatRatio, float pressure, Point dragCoefficient, const float* pixelsToMeters);

	~GasBody() override;

	float GetVolume(InUnit unit);

	float GetDensity(InUnit unit);

	float GetHeatRatio() const;

	float GetPressure() const;

	void SetDensity(float density, InUnit unit);

	void SetHeatRatio(float heatRatio);

	Point GetDragCoefficient() const
	{
		return dragCoefficient;
	}

	Point GetLiftCoefficient() const
	{
		return liftCoefficient;
	}

private:

	float heatRatio = 0;
	float pressure = 0;
	Point dragCoefficient = {1.0f, 1.0f};
	Point liftCoefficient = {0.0f, 0.0f};
};
