#pragma once

#include "TileHeader.h"
#include "Grid.h"
#include "Render.h"
#include "Window.h"

class TileManager
{
public:

	TileManager(Point size, Render* render, Window* window, NPL* physics);

	bool Update(float dt);

	bool Draw(float dt);

	void SetTile(TileType type);

	void PlaceTile(Point position);

private:

	TileType currentType = TileType::NO_TILE;
	float tileSize = 0;
	Grid<Tile*>* grid = nullptr;

	const Render* render = nullptr;
	const Window* window = nullptr;
	NPL* physics = nullptr;

};