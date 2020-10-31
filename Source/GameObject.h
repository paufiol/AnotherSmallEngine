#pragma once
#include <string>
#include <vector>

class Component;
//enum class ComponentType;

class GameObject {
//Properties
public:
	std::string name;
private:
	std::vector<Component*> components;
	bool active;

//Methods
public:
	GameObject(std::string name);

	bool Enable(); //Start up + bool toggle
	bool Disable();//Clean up + bool toggle
	
	void Update();

	Component* AddComponent(Component* component);
private:
	
};