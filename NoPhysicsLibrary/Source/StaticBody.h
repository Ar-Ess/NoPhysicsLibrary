#pragma once

#include "Body.h"

class StaticBody : public Body
{
public: // Methods

	StaticBody(Rect rect = { 0.0f, 0.0f, 1.0f, 1.0f }, float mass = 1.0f);

	~StaticBody() override;
};
