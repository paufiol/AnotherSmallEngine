#pragma once
#include "Component.h"
#include "Globals.h"
#include "glmath.h"
#include "Dependencies/MathGeoLib/include/Geometry/AABB.h"

struct ResourceMesh;

class ComponentMesh : public Component {
	//Properties
public:

private:
	ResourceMesh* mesh = nullptr;
	std::string path = ""; 

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
	void SetResourceID(uint32 ID);
	//uint32 GetResourceUID() const { return mesh->UID; }

	ComponentMesh(GameObject* parent); 
	ComponentMesh(GameObject* parent, const char* meshPath, ResourceMesh* _mesh = nullptr);

	const AABB& GetAABB() const;

private:
};