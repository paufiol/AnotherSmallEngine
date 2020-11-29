#pragma once
#include "ComponentMesh.h"
#include "GameObject.h"

#include "ResourceMesh.h"
#include "Dependencies/ImGUI/imgui.h"

ComponentMesh::ComponentMesh(GameObject* owner) : Component(owner)
{
	//parent->AddComponent(this);
	mesh = nullptr; 
	path = "No path!";
	type = ComponentType::Mesh;
};
ComponentMesh::ComponentMesh(GameObject* owner, const char* meshPath, ResourceMesh* _mesh) : Component(owner)
{
	mesh = _mesh;
	path = meshPath;
	type = ComponentType::Mesh;
}

void ComponentMesh::Enable() {

}

void ComponentMesh::Disable() {

}

void ComponentMesh::Update() {

}

void ComponentMesh::CleanUp()
{
	//delete[] mesh->indices;
	//delete[] mesh->normals;
	//delete[] mesh->vertices;
}

void ComponentMesh::DrawInspector() {
	
	if(ImGui::CollapsingHeader("Component Mesh"))
	{
		ImGui::Text("Path: %s", path.c_str());
		ImGui::Text("Index: %d", mesh->size[0]);
		ImGui::Text("Vertices: %d", mesh->size[1]);
		ImGui::Text("Normals: %d", mesh->size[2]);
		ImGui::Text("texture: %d", mesh->size[3]);
	}
}

void ComponentMesh::SetMesh(ResourceMesh* mesh)
{
	this->mesh = mesh;
}

void ComponentMesh::SetPath(std::string path)
{
	this->path = path;
}