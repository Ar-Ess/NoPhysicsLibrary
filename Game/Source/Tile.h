#pragma once
#include "NoPhysicsLibrary.h"
#include "TileTypeEnum.h"

class Tile
{
public:

	virtual ~Tile();

	virtual void Update();

	virtual void Draw();

	virtual bool Delete(NPL* physics);

protected:

	TileType type = TileType::NO_TILE;
	unsigned int id = 0;

};