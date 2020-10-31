#pragma once
#include "Component.h"
#include "glmath.h"

class ComponentMesh : public Component {
	//Properties
public:

private:

	//Methods
public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	ComponentMesh(GameObject* parent); //DEFAULTS TO 0,0
	//ComponentMesh(GameObject* parent, vec3 position, vec3 rotation, vec3 scale);
private:
};