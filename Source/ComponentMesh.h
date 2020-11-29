#pragma once
#include "Component.h"
#include "glmath.h"

struct ResourceMesh;

class ComponentMesh : public Component {
	//Properties
public:

private:
	ResourceMesh* mesh;
	std::string path; 

	//Methods
public:
	void Enable() override;
	void Update() override;
	void Disable() override;
	void DrawInspector() override;
	void CleanUp() override;

	ResourceMesh* GetMesh() const { return mesh; };
	std::string  GetPath() const { return path; };

	void SetMesh(ResourceMesh* mesh);
	void SetPath(std::string path);

	ComponentMesh(GameObject* parent); 
	ComponentMesh(GameObject* parent, const char* meshPath, ResourceMesh* _mesh = nullptr);
private:
};