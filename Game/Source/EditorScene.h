#pragma once

#include "Scene.h"
#include "Player.h"
#include "TileManager.h"

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

	bool UpdateGeneralInputs();

private:

	// Editor
	TileManager* map = nullptr;
	bool editMode = true;

	// Physics
	NoPhysicsLibrary* physics = nullptr;
	Player* player = nullptr;

};