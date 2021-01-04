#pragma once
#include "Application.h"
#include "ModuleResource.h"
#include "ComponentMaterial.h"
#include "ResourceMaterial.h"
#include "ResourceShader.h"
#include "ImporterShader.h"
#include "GameObject.h"
#include "ModuleEditor.h"
#include <string>
#include "Dependencies/ImGUI/imgui.h"

#include "ComponentTransform.h"

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


		std::map<uint32, ResourceTexture*> texturesInMemory = App->resources->GetTexturesInMemory();

		std::map<uint32, ResourceTexture*>::iterator nameIt = texturesInMemory.begin();

		const char* combo_label = nameIt->second->name.c_str();
		if (ImGui::BeginCombo("combo 1", combo_label, ImGuiComboFlags_PopupAlignLeft))
		{
			for (std::map<uint32, ResourceTexture*>::iterator item = texturesInMemory.begin(); item != texturesInMemory.end(); item++)
			{
				const bool is_selected = (nameIt == item);
				if (ImGui::Selectable(item->second->name.c_str(), is_selected))
					nameIt = item;
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}


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

		ResourceShader* shader = rMaterial->GetShader();

		for (uint i = 0; i < shader->uniforms.size(); i++)
		{
			switch (shader->uniforms[i].uniformType)
			{
			case  UniformType::INT:	ImGui::DragInt(shader->uniforms[i].name.c_str(), &shader->uniforms[i].integer, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			case  UniformType::FLOAT: ImGui::DragFloat(shader->uniforms[i].name.c_str(), &shader->uniforms[i].floatNumber, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			case  UniformType::INT_VEC2: ImGui::DragFloat2(shader->uniforms[i].name.c_str(), (float*)&shader->uniforms[i].vec2, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			case  UniformType::INT_VEC3: ImGui::DragFloat3(shader->uniforms[i].name.c_str(), (float*)&shader->uniforms[i].vec3, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			case  UniformType::INT_VEC4: ImGui::DragFloat4(shader->uniforms[i].name.c_str(), (float*)&shader->uniforms[i].vec4, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			case  UniformType::FLOAT_VEC2: ImGui::DragFloat2(shader->uniforms[i].name.c_str(), (float*)&shader->uniforms[i].vec2, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			case  UniformType::FLOAT_VEC3: ImGui::DragFloat3(shader->uniforms[i].name.c_str(), (float*)&shader->uniforms[i].vec3, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			case  UniformType::FLOAT_VEC4: ImGui::DragFloat4(shader->uniforms[i].name.c_str(), (float*)&shader->uniforms[i].vec4, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			case UniformType::MATRIX4: ImGui::DragFloat4(shader->uniforms[i].name.c_str(), shader->uniforms[i].matrix4.ToEulerXYZ().ptr(), 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			}		
		}
	}
}

