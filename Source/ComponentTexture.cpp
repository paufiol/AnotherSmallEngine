#pragma once
#include "ComponentTexture.h"
#include "ResourceMaterial.h"
#include "GameObject.h"
#include <string>
#include "Dependencies/ImGUI/imgui.h"

ComponentTexture::ComponentTexture(GameObject* owner) : Component(owner)
{
	rMaterial = new ResourceMaterial();
	rMaterial->SetId(-1);
	rMaterial->SetPath( "No Path!");
	type = ComponentType::Material;
};

ComponentTexture::ComponentTexture(GameObject* owner, const char* texturePath, uint id) : Component(owner)
{
	rMaterial = new ResourceMaterial();
	rMaterial->SetId(id);
	rMaterial->SetPath(texturePath);
	type = ComponentType::Material;
}

ComponentTexture::ComponentTexture(GameObject* owner, ResourceMaterial* _rMaterial) : Component(owner)
{
	rMaterial = _rMaterial;
	type = ComponentType::Material;
}

void ComponentTexture::Enable() {

}

void ComponentTexture::Disable() {

}

void ComponentTexture::Update() {

}

void ComponentTexture::CleanUp()
{
	//delete rMaterial;
}

void ComponentTexture::DrawInspector() {

	if (ImGui::CollapsingHeader("Component Texture"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::Text("Path: ");
		ImGui::SameLine();
		ImGui::TextColored(GREEN,"%s", rMaterial->GetPath().c_str());
		ImGui::Text("Id: ");
		ImGui::SameLine();
		ImGui::TextColored(YELLOW, "%d", rMaterial->GetId());
	}
}

