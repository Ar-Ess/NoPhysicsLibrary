#pragma once

#include "TileHeader.h"
#include "Grid.h"
#include "Render.h"
#include "Window.h"

class TileManager
{
public:

	TileManager(Point size, Render* render, Window* window);

	bool Update(float dt);

	bool Draw(float dt);

	void SetTile(TileType type);

private:

	TileType currentType = TileType::NO_TYLE;
	Grid<Tile*>* grid = nullptr;

	const Render* render = nullptr;
	const Window* window = nullptr;

};