#pragma once

#include "Body.h"

class LiquidBody : public Body
{
public:

	LiquidBody(Rect rect, float mass, Flag* bodiesConfig, const float* pixelsToMeters);

	~LiquidBody() override;

};
