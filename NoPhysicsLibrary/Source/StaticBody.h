#pragma once

#include "Body.h"

class StaticBody : public Body
{
public: // Methods

	StaticBody(Rect rect, float mass, Flag* bodiesConfig, const float* pixelsToMeters);

	~StaticBody() override;
};
