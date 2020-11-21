#pragma once
#include <string>
#include <vector>

class Component;
class ComponentTransform;
class ComponentMesh;
class ComponentTexture;
enum class ComponentType;

using namespace std;
class GameObject {
//Properties
public:
	std::string name;

	std::vector<Component*> components;
	bool active;

	GameObject* parent;
	std::vector<GameObject*> children;

	ComponentTransform* transform = nullptr;
	ComponentTexture* texture = nullptr;
private:


//Methods
public:
	GameObject(string name);

	bool Enable(); //Start up + bool toggle
	bool Disable();//Clean up + bool toggle
	
	void Update();

	void SetParent(GameObject* _parent);
	void SetName(const char* _name);

	Component* AddComponent(Component* component);
	Component* GetComponent(ComponentType type);
	std::vector<Component*> GetComponents(ComponentType type);
	GameObject* AddChildren(GameObject* children);
	bool EraseChildren(GameObject* children);
	
	bool selected = false;
private:
	
};