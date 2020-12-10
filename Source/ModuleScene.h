#pragma once
#include "Module.h"
#include "Globals.h"
#include "Dependencies/MathGeoLib/include/Geometry/LineSegment.h"

class GameObject;
class Primitive;
class ResourceMesh;

class ModuleScene : public Module
{
public:
	ModuleScene(bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp();

	void SaveScene();

	void AddGameObject(GameObject* object);
	GameObject* CreateGameObject(std::string name, GameObject* parent = nullptr);
	void SelectObject(GameObject* object);
	void DeleteGameObject(GameObject* object);

	GameObject* CreateGameCamera();

	void TestGameObjectSelection(const LineSegment& ray);

private:
	bool PerTriangleTest(const ResourceMesh* mesh);

private:
	std::vector<Primitive*> primitives;

public:
	std::vector<GameObject*> game_objects;

	GameObject* root_object = nullptr;
	GameObject* selected_object = nullptr;
};
