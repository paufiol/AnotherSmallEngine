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
	void DrawInspector() override; 

	Mesh* GetMesh() const { return mesh; };
	std::string  GetPath() const { return path; };

	void SetMesh(Mesh* mesh);
	void SetPath(std::string path);

	ComponentMesh(GameObject* parent); 
private:
};