#pragma once

enum BodyState
{
	// Is inside a gas
	IN_GAS = 0,
	// Is touching the ground
	ON_GROUND,
	// Is touching the roof
	ON_ROOF,
	// Is touching a left wall
	ON_LEFT,
	// Is touching a right wall
	ON_RIGHT,
	// Is in a liquid
	IN_LIQUID,
	// Is not touching any wall/liquid
	FLOATING,
	// Is moving
	MOVING
};
