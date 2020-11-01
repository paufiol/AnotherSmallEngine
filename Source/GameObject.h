#pragma once
#include <string>
#include <vector>

class Component;
enum class ComponentType;

class GameObject {
//Properties
public:
	std::string name;

	std::vector<Component*> components;
	bool active;

	GameObject* parent;
	
private:
	std::vector<GameObject*> children;

//Methods
public:
	GameObject(std::string name);

	bool Enable(); //Start up + bool toggle
	bool Disable();//Clean up + bool toggle
	
	void Update();

	Component* AddComponent(Component* component);
	Component* GetComponent(ComponentType type);

	void AppendChildren(GameObject* children);

private:
	
};