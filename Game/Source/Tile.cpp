#include "Tile.h"

Tile::~Tile()
{
}

void Tile::Update()
{
}

void Tile::Draw()
{
}

bool Tile::Delete(NPL* physics)
{
	return physics->DestroyBody(id);
}
