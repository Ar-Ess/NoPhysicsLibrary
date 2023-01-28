#include "TileManager.h"

TileManager::TileManager(Point size, Render* render, Window* window)
{
	grid = new Grid<Tile*>(size);
	this->render = render;
	this->window = window;
}

bool TileManager::Update(float dt)
{
	for (unsigned int i = 0; i < grid->Size(); ++i) (*grid)[i]->Update();

	return true;
}

bool TileManager::Draw(float dt)
{
	for (unsigned int i = 0; i < grid->Size(); ++i) (*grid)[i]->Draw();

	render->DrawGrid({ 0, 0, window->GetSize().Multiply(1.0f, 2) }, { (float)grid->Width(), (float)grid->Heigth() });

	return true;
}
