#pragma once
#include "Application.h"
#include "ComponentMaterial.h"
#include "ResourceMaterial.h"
#include "ResourceShader.h"
#include "GameObject.h"
#include "ModuleEditor.h"
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
		ImGui::Text("Texture:");
		ImGui::SameLine();
		ImGui::TextColored(GREEN,"%s", rMaterial->GetTexture()->name.c_str());
		if (rMaterial->GetTexture() != nullptr)
		{
			ImGui::Text("Path: ");
			ImGui::SameLine();
			ImGui::TextColored(GREEN,"%s", rMaterial->GetTexture()->assetsFile.c_str());
			ImGui::Text("Id: ");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%d", rMaterial->GetId());
		}
		else
		{
			ImGui::Text("Texture is nullptr");
		}
		ImGui::Separator();
		ImGui::Text("Material:");
		ImGui::SameLine();
		ImGui::TextColored(GREEN, "%s", rMaterial->name.c_str());
		if (rMaterial)
		{
			ImGui::Text("Color: ");
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%d, ", rMaterial->GetColor().r);
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%d, ", rMaterial->GetColor().g);
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%d, ", rMaterial->GetColor().b);
			ImGui::SameLine();
			ImGui::TextColored(YELLOW, "%d", rMaterial->GetColor().a);
		}
		ImGui::Separator();
		ImGui::Text("Shader:");
		ImGui::SameLine();
		ImGui::TextColored(GREEN, "%s", rMaterial->GetShader()->name.c_str());
		if (ImGui::Button("Edit Shader"))
		{
			App->editor->CallTextEditor(this->rMaterial);
		}
	}
}

