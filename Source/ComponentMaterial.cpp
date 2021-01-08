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
		std::map<uint32, ResourceTexture*> texturesInMemory = App->resources->GetTexturesInMemory();

		std::map<uint32, ResourceTexture*>::iterator nameIt = texturesInMemory.begin();

		if (ImGui::BeginCombo("Texture", rMaterial->GetTexture()->name.c_str(), ImGuiComboFlags_PopupAlignLeft))
		{
			for (std::map<uint32, ResourceTexture*>::iterator item = texturesInMemory.begin(); item != texturesInMemory.end(); item++)
			{
				const bool is_selected = (nameIt == item);
				if (ImGui::Selectable(item->second->name.c_str(), is_selected))
				{
					//To delete
					if (item->second->id == 0)
					{
						ResourceTexture* newTexture = (ResourceTexture*)App->resources->LoadResource(item->second->UID);
						rMaterial->SetTexture(newTexture);
					}
					else
					{
						rMaterial->SetTexture(item->second);
					}

					nameIt = item;
				}
				
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
		

		std::map<uint32, ResourceShader*> shadersInMemory = App->resources->GetShadersInMemory();

		std::map<uint32, ResourceShader*>::iterator shaderName = shadersInMemory.begin();

		if (ImGui::BeginCombo("Shader", rMaterial->GetShader()->name.c_str(), ImGuiComboFlags_PopupAlignLeft))
		{
			for (std::map<uint32, ResourceShader*>::iterator iterator = shadersInMemory.begin(); iterator != shadersInMemory.end(); iterator++)
			{
				const bool selectedShader = (shaderName == iterator);
				if (ImGui::Selectable(iterator->second->name.c_str(), selectedShader))
				{
					if (iterator->second->shaderProgramID == 0)
					{
						ResourceShader* newShader = (ResourceShader*)App->resources->LoadResource(iterator->second->UID);
						rMaterial->SetShader(newShader);
					}
					else
					{
						rMaterial->SetShader(iterator->second);
					}

					shaderName = iterator;
				}
			}
			ImGui::EndCombo();
		}


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
			case  UniformType::INT_VEC2: ImGui::DragInt2(shader->uniforms[i].name.c_str(), (int*)&shader->uniforms[i].vec2, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			case  UniformType::INT_VEC3: ImGui::DragInt3(shader->uniforms[i].name.c_str(), (int*)&shader->uniforms[i].vec3, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			case  UniformType::INT_VEC4: ImGui::DragInt4(shader->uniforms[i].name.c_str(), (int*)&shader->uniforms[i].vec4, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			case  UniformType::FLOAT_VEC2: ImGui::DragFloat2(shader->uniforms[i].name.c_str(), (float*)&shader->uniforms[i].vec2, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			case  UniformType::FLOAT_VEC3: ImGui::DragFloat3(shader->uniforms[i].name.c_str(), (float*)&shader->uniforms[i].vec3, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			case  UniformType::FLOAT_VEC4: ImGui::DragFloat4(shader->uniforms[i].name.c_str(), (float*)&shader->uniforms[i].vec4, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			case UniformType::MATRIX4: ImGui::DragFloat4(shader->uniforms[i].name.c_str(), shader->uniforms[i].matrix4.ToEulerXYZ().ptr(), 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			}		
		}

		if (!rMaterial->GetShader()->uniforms.empty())
		{
			if (ImGui::Button("Save Uniforms"))
			{
				App->resources->SaveResource(rMaterial->GetShader());
			}
		}

	}
}

