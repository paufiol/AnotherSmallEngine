#pragma once
#include "ComponentTexture.h"
#include "GameObject.h"
#include <string>
#include "ModuleImporter.h" //TODO: NEEDED ONLY BECAUSE OF MESH CLASS-> Turn Mesh Class into stand alone file.
#include "Dependencies/ImGUI/imgui.h"

ComponentTexture::ComponentTexture(GameObject* parent) : Component(parent)
{
	texture = new Texture();
	texture->id = -1;
	texture->path = "Default Path";
	type = ComponentType::Material;
};

void ComponentTexture::Enable() {

}

void ComponentTexture::Disable() {

}

void ComponentTexture::Update() {

}

void ComponentTexture::DrawInspector() {

	if (ImGui::CollapsingHeader("Component Texture"))
	{
		ImGui::Text("Path: %s", texture->path);
		ImGui::Text("Id: %d", texture->id);
	}
}

void ComponentTexture::SetTexture(Texture* texture)
{
	this->texture->id = texture->id;
	this->texture->path = texture->path;
}

void ComponentTexture::SetTexture(uint id, const char* path)
{
	this->texture->id = id;
	this->texture->path = path;
}

Texture* ComponentTexture::GetTexture()
{
	if (this != nullptr) {
		Texture ret;

		ret.id = this->texture->id;
		ret.path = this->texture->path;

		return &ret;
	}
	return nullptr;
}
