#pragma once

#include "Body.h"

class LiquidBody : public Body
{
public:

	LiquidBody(Rect rect, float mass, const float* pixelsToMeters);

	~LiquidBody() override;

};
