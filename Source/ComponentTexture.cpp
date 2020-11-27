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
	rMaterial->SetPath( "Default Path");
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

void ComponentTexture::DrawInspector() {

	if (ImGui::CollapsingHeader("Component Texture"))
	{
		ImGui::Text("Path: %s", rMaterial->GetPath());
		ImGui::Text("Id: %d", rMaterial->GetId());
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
