#pragma once
#include "ComponentMesh.h"
#include "GameObject.h"

#include "ModuleImporter.h" //TODO: NEEDED ONLY BECAUSE OF MESH CLASS-> Turn Mesh Class into stand alone file.
#include "Dependencies/ImGUI/imgui.h"

ComponentMesh::ComponentMesh(GameObject* parent) : Component(parent)
{
	//parent->AddComponent(this);
	mesh = nullptr; 
	path = "No path!";
	type = ComponentType::Mesh;
};

void ComponentMesh::Enable() {

}

void ComponentMesh::Disable() {

}

void ComponentMesh::Update() {

}

void ComponentMesh::DrawInspector() {
	
	if(ImGui::CollapsingHeader("Component Mesh"))
	{
		ImGui::Text("Path: %s", path);
		ImGui::Text("Index: %d", mesh->size[0]);
		ImGui::Text("Vertices: %d", mesh->size[1]);
		ImGui::Text("Normals: %d", mesh->size[2]);
		ImGui::Text("texture: %d", mesh->size[3]);
	}
}

void ComponentMesh::SetMesh(Mesh* mesh)
{
	this->mesh = mesh;
}

void ComponentMesh::SetPath(std::string path)
{
	this->path = path;
}