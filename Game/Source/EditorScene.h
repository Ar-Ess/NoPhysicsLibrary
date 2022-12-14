#pragma once

#include "Scene.h"
#include "NoPhysicsLibrary.h"
#include "Grid.h"

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
	Grid<int*> grid = Grid<int*>(24, 14);
	bool editMode = true;

	// Physics
	NPL* physics = nullptr;
	DynamicBody* player = nullptr;

};