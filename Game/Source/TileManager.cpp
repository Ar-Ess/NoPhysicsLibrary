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

bool TileManager::Draw(float dt, bool lines, bool debug)
{
	for (unsigned int i = 0; i < grid->Size(); ++i) (*grid)[i]->Draw();

	if (lines) render->DrawGrid({ 0, 0, window->GetSize().Multiply(1.f, 2) }, { (float)grid->Width(), (float)grid->Heigth() }, {255, 255, 255, 70});

	DrawDebug(debug);

	return true;
}

void TileManager::SetTile(TileType type)
{
	currentType = type;
}

bool TileManager::UpdateTile(Point position)
{
	if (currentType == TileType::NO_TILE) return false;
	if (position.IsNegative()) return false;

	int x = MathUtils::Floor(position.x / tileSize);
	int y = MathUtils::Floor(position.y / tileSize);

	if (x > grid->Width() - 1|| y > grid->Heigth() - 1) return false;

	if (currentType == TileType::ERASE_TILE) return EraseTile(x, y);

	if (!grid->Empty(x, y)) return false;

	
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

	return true;
}

void TileManager::DrawDebug(bool debug)
{
	if (!debug) return;

	// Debug draw
	unsigned int size = physics->Get()->BodiesCount();
	for (unsigned int i = 0; i < size; ++i)
	{
		const Body* b = physics->Get()->Bodies(i);
		SDL_Color color = { 0, 0, 0, 100 };

		switch (b->GetClass())
		{
		case BodyClass::STATIC_BODY:  color = { 255,   0,   0, color.a }; break;
		case BodyClass::DYNAMIC_BODY: color = { 0, 255,   0, color.a }; break;
		case BodyClass::LIQUID_BODY:  color = { 100, 100, 255, color.a }; break;
		case BodyClass::GAS_BODY:     color = { 255, 255, 255, (Uint8)(color.a - 20) }; break;
		}
		render->DrawRectangle(b->GetRect(InUnit::IN_PIXELS), color);
		//render->DrawRectangle(Rect{ b->GetEmissionPoint(InUnit::IN_PIXELS).Apply({-3.0f, -3}), 6, 6 }, { 155, 255, 155, 255 });
	}

}

bool TileManager::EraseTile(int x, int y)
{
	Tile* tile = grid->At(x, y);
	if (!tile) return false;

	return tile->Delete(physics) && grid->Erase(x, y);
}
