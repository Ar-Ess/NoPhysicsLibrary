#include "TileManager.h"
#include "MathUtils.h"

TileManager::TileManager(Point size, Render* render, Window* window, NPL* physics)
{
	grid = new Grid<Tile*>(size);
	this->render = render;
	this->window = window;
	this->physics = physics;
	tileSize = { window->GetSize().x / size.x };
}

bool TileManager::Update(float dt)
{
	for (unsigned int i = 0; i < grid->Size(); ++i) (*grid)[i]->Update();

	return true;
}

bool TileManager::Draw(float dt)
{
	for (unsigned int i = 0; i < grid->Size(); ++i) (*grid)[i]->Draw();

	render->DrawGrid({ 0, 0, window->GetSize().Multiply(1.f, 2) }, { (float)grid->Width(), (float)grid->Heigth() });

	return true;
}

void TileManager::SetTile(TileType type)
{
	currentType = type;
}

void TileManager::PlaceTile(Point position)
{
	if (currentType == TileType::NO_TILE) return;
	if (position.IsNegative()) return;

	int x = MathUtils::Floor(position.x / tileSize);
	int y = MathUtils::Floor(position.y / tileSize);

	if (!grid->Empty(x, y)) return;
	
	Rect rect = {x * tileSize, y * tileSize, tileSize, tileSize };
	Tile* tile = nullptr;

	switch (currentType)
	{
	case TileType::GROUND_TILE:
	{
		StaticBody* body = physics->CreateBody(rect)->Static();
		tile = new GroundTile(body);
		break;
	}
	}

	grid->Set(tile, x, y);
}
