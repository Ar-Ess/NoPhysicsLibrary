#pragma once

#include "DirectionEnum.h"

struct GhostSlot
{
	GhostSlot() 
	{
		this->surface = 0;
		this->dir = Direction::NO_DIR;
		this->slot = -1;
	}
	GhostSlot(Direction direct, int surf, int slt)
	{
		dir = direct;
		surface = surf;
		slot = slt;
	}
	~GhostSlot()
	{

	}

	int surface = 0;
	Direction dir = Direction::NO_DIR;
	int slot = -1;

	bool operator >(const GhostSlot& a)
	{
		if (surface > a.surface) return true;
		return false;
	}
	bool operator <(const GhostSlot& a)
	{
		if (surface < a.surface) return true;
		return false;
	}
};
