#pragma once
#include "Globals.h"
#include "Random.h"
#include "Dependencies/MathGeoLib/include/Geometry/AABB.h"
#include "Dependencies/MathGeoLib/include/Geometry/OBB.h"

class Component;
class ComponentTransform;
class ComponentMesh;
class ComponentTexture;
enum class ComponentType;

class ResourceScene;

class GameObject {
//Properties
public:
	std::string name;


	GameObject* parent;
	std::vector<GameObject*> children;

	std::vector<Component*> components;
	ComponentTransform* transform = nullptr;
	ComponentTexture* texture = nullptr;
	bool active;


	AABB	aabb;
	OBB		obb;

private:

	uint32 	UID;

	Random randomNum;
//Methods
public:
	GameObject(std::string name = "Game_Object");
	~GameObject();

	bool Enable(); //Start up + bool toggle
	bool Disable();//Clean up + bool toggle
	
	void Update();
	void CleanUp();

	void SetParent(GameObject* _parent);
	void SetName(const char* _name);

	const uint32 GetUID() const;
	void SetUID(uint32 UID);

	//BoundingBox Handling
	const AABB& GetAABB() const;
	const OBB& GetOBB() const;

	bool IsSelected();
	bool IsRootObject();

	//Component handling
	Component* AddComponent(Component* component);
	Component* GetComponent(ComponentType type);
	std::vector<Component*> GetComponents(ComponentType type);
	void EraseComponents();

	//Children handling
	GameObject* AddChildren(GameObject* children);
	void EraseChild(GameObject* child, bool deleteChild = true);	//Delete specific child
	void EraseAllChildren();			//Delete them all
	void FillGameObjectArray(GameObject* gameObject, std::vector<GameObject*> array);

	void CreateGOfromResource(ResourceScene* model);


	bool selected = false;
private:
	void UpdateAABB();
};