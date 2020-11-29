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
	GameObject* owner;

private:

	
	//Methods
public:
	virtual void Enable() {};
	virtual void Update() {};
	virtual void Disable() {};

	virtual void CleanUp() {};

	virtual void DrawInspector() {};
	Component(GameObject* owner) : active(true), owner(owner), type(ComponentType::Error) {};
private:

};