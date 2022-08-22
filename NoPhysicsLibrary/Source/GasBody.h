#pragma once

#include "Body.h"

class GasBody : public Body
{
public:

	GasBody(Rect rect, float mass);

	~GasBody() override;
};
