#pragma once
#include "ComponentMaterial.h"
#include "ResourceMaterial.h"
#include "GameObject.h"
#include <string>
#include "Dependencies/ImGUI/imgui.h"

ComponentMaterial::ComponentMaterial(GameObject* owner) : Component(owner)
{
	rMaterial = new ResourceMaterial();
	//rMaterial->SetId(-1);
	//rMaterial->SetPath( "Default Path");
	type = ComponentType::Material;
};

ComponentMaterial::ComponentMaterial(GameObject* owner, const char* texturePath, uint id) : Component(owner)
{
	rMaterial = new ResourceMaterial();
	rMaterial->SetId(id);
	rMaterial->SetPath(texturePath);
	type = ComponentType::Material;
}
ComponentMaterial::ComponentMaterial(GameObject* owner, ResourceMaterial* _rMaterial) : Component(owner)
{
	rMaterial = _rMaterial;
	type = ComponentType::Material;
}


void ComponentMaterial::Enable() {

}

void ComponentMaterial::Disable() {

}

void ComponentMaterial::Update() {

}

void ComponentMaterial::CleanUp()
{
	rMaterial->SetId(0);
	rMaterial = nullptr;
	delete rMaterial;
}

void ComponentMaterial::DrawInspector() {

	if (ImGui::CollapsingHeader("Component Material"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		if (rMaterial->GetTexture() != nullptr)
		{
			ImGui::Text("Path: ");
			ImGui::SameLine();
			ImGui::TextColored(GREEN,"%s", rMaterial->GetPath().c_str());
			ImGui::Text("Id: ");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%d", rMaterial->GetId());
		}
		else
		{
			ImGui::Text("Texture is nullptr");
		}
	}
}

//void ComponentTexture::SetTexture(Texture* texture)
//{
//	this->texture->id = texture->id;
//	this->texture->path = texture->path;
//}
//
//void ComponentTexture::SetTexture(uint id, const char* path)
//{
//	this->texture->id = id;
//	this->texture->path = path;
//}
//
//Texture* ComponentTexture::GetTexture()
//{
//	if (this != nullptr) {
//		Texture ret;
//
//		ret.id = this->texture->id;
//		ret.path = this->texture->path;
//
//		return &ret;
//	}
//	return nullptr;
//}
