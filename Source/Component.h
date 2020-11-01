#pragma once
#include <string>

class GameObject;

enum class ComponentType {
	Error = -1,
	Transform,
	Mesh,
	Material,
};

class Component {
	//Properties
public:
	ComponentType type;
	bool active;
	GameObject* parent;

private:

	
	//Methods
public:
	virtual void Enable() {};
	virtual void Update() {};
	virtual void Disable() {};

	virtual void DrawInspector() {};
	Component(GameObject* parent) : active(true), parent(parent), type(ComponentType::Error) {};
private:

};