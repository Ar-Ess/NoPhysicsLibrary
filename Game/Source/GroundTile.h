#pragma once

#include "Tile.h"

class GroundTile : public Tile
{
public:

	GroundTile(StaticBody* body);

	void Update() override;

	void Draw() override;

private:

	StaticBody* body = nullptr;

};