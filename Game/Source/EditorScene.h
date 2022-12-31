#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"
#include "Grid.h"
#include "Tile.h"

class EditorScene : public Scene
{
public:

	EditorScene();

	~EditorScene() override;

	bool Start() override;

	bool Update(float dt) override;

	bool Draw(float dt) override;

	bool CleanUp() override;

private:

	bool UpdateEditMode(float dt);
	bool DrawEditMode(float dt);

	bool UpdatePlayMode(float dt);
	bool DrawPlayMode(float dt);

private:

	// Editor
	Grid<Tile*> grid = Grid<Tile*>(24, 27);
	bool editMode = true;

	// Physics
	NPL* physics = nullptr;
	DynamicBody* player = nullptr;

};