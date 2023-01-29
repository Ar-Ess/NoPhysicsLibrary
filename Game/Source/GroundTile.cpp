#include "GroundTile.h"

GroundTile::GroundTile(StaticBody* body)
{
	this->body = body;
	this->type = TileType::GROUND_TILE;
	this->id = body->GetId();
}

GroundTile::~GroundTile()
{
	body = nullptr;
}

void GroundTile::Update()
{

}

void GroundTile::Draw()
{
}
