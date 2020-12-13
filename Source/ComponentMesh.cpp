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

const AABB& ComponentMesh::GetAABB() const
{
	if (mesh->ID != 0)//Uncertain on what ID 0 is
		return mesh->aabb;
	else
		return AABB(float3(-.5f, -.5f, .5f), float3(.5f, .5f, .5f));
}

void ComponentMesh::Enable() {

}

void ComponentMesh::Disable() {

}

void ComponentMesh::Update() {

}

void ComponentMesh::CleanUp()
{
	delete[] mesh->indices;
	delete[] mesh->normals;
	delete[] mesh->vertices;
}

void ComponentMesh::DrawInspector() {
	
	if(ImGui::CollapsingHeader("Component Mesh"), ImGuiTreeNodeFlags_DefaultOpen)
	{

		if (mesh != nullptr)
		{
		
			ImGui::Text("Path: ");
			ImGui::SameLine();
			ImGui::TextColored(GREEN, "%s", path.c_str());

			ImGui::Text("Index: ");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%d", mesh->size[0]);

			ImGui::Text("Vertices: ");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%d", mesh->size[1]);

			ImGui::Text("Normals: ");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%d", mesh->size[2]);

			ImGui::Text("Texture Coords: ");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%d", mesh->size[3]);

		}
		else
		{
			ImGui::Text("Mesh is nullptr");
		}
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

void ComponentMesh::SetResourceID(uint32 ID)
{
	mesh->UID = ID;
}
