#pragma once
#include "ComponentMaterial.h"
#include "ResourceMaterial.h"
#include "ResourceShader.h"
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

		Color color;
		uint uinformLoc = glGetUniformLocation(rMaterial->GetShader()->shaderProgramID, "inColor");
		glGetUniformfv(rMaterial->GetShader()->shaderProgramID, uinformLoc, &color);
		if (ImGui::DragFloat4("Uniform Color", (float*)&color, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None)) 
		{ 
			rMaterial->SetColor(color);
			rMaterial->GetShader()->SetUniform4f("inColor", (GLfloat*)&color); 
		}

		ImGui::Button("Edit Shader");
		
	}
}

