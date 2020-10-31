#pragma once
#include "Component.h"
#include "glmath.h"

class ComponentTransform : public Component {
	//Properties
public:
	vec3 position;
	vec3 rotation;
	vec3 scale;
private:

	//Methods
public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	ComponentTransform(GameObject* parent); //DEFAULTS TO 0,0
	ComponentTransform(GameObject* parent, vec3 position, vec3 rotation, vec3 scale);
private:
};