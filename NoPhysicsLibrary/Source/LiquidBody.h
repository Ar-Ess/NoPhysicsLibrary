#pragma once

#include "Body.h"

class LiquidBody : public Body
{
public:

	LiquidBody(Rect rect, float mass, float buoyancy, const float* pixelsToMeters);

	~LiquidBody() override;

	float GetVolume(InUnit unit);

	float GetDensity(InUnit unit);

	float GetBuoyancy();

	void SetDensity(float density, InUnit unit);

	void SetBuoyancy(float buoyancy);

	Point GetLiftCoefficient() const
	{
		return liftCoefficient;
	}

	Point GetDragCoefficient() const
	{
		return dragCoefficient;
	}

private:

	float buoyancy = 0.0f;
	Point liftCoefficient = {0.1f, 0.0f};
	Point dragCoefficient = {1.0f, 0.0f};
};
