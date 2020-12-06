#pragma once
#include "Dependencies/MathGeoLib/include/Math/float4x4.h"

class GameObject;


enum class ComponentType {
	Error = -1,
	Transform,
	Mesh,
	Material,
	Camera
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

	virtual void OnUpdateTransform(const float4x4& global, const float4x4& parent_global = float4x4::identity);

	Component(GameObject* owner) : active(true), owner(owner), type(ComponentType::Error) {};
private:

};