#pragma once

#include "Body.h"

class LiquidBody : public Body
{
public:

	LiquidBody(Rect rect, float mass, float buoyancy, Flag* bodiesConfig, const float* pixelsToMeters);

	~LiquidBody() override;

	float GetVolume(InUnit unit);

	float GetDensity(InUnit unit);

	float GetBuoyancy();

private:

	float buoyancy = 0.0f;

};
