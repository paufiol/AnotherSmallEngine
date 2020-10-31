#pragma once
#include "Component.h"
#include "glmath.h"

struct Mesh;

class ComponentMesh : public Component {
	//Properties
public:

private:
	Mesh* mesh;
	std::string path; 

	//Methods
public:
	void Enable() override;
	void Update() override;
	void Disable() override;

	Mesh* GetMesh() const { return this->mesh; };

	ComponentMesh(GameObject* parent); //DEFAULTS TO 0,0
	//ComponentMesh(GameObject* parent, vec3 position, vec3 rotation, vec3 scale);
private:
};