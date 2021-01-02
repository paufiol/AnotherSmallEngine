#pragma once
#include "Application.h"
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
			case  UniformType::FLOAT_VEC4:
				if (ImGui::DragFloat4(shader->uniforms[i].name.c_str(), (float*)&shader->uniforms[i].vec4, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None))
				{
					//shader->SetUniformVec4f(shader->uniforms[i].name.c_str(), (GLfloat*)&shader->uniforms[i].vec4);
					if (shader->uniforms[i].name == "inColor")
					{
						Color color;
						color.r = shader->uniforms[i].vec4.x;
						color.g = shader->uniforms[i].vec4.y;
						color.b = shader->uniforms[i].vec4.z;
						color.a = shader->uniforms[i].vec4.w;
						rMaterial->SetColor(color);
					}

				}
				break;
			case UniformType::MATRIX4: ImGui::DragFloat4(shader->uniforms[i].name.c_str(), shader->uniforms[i].matrix4.ToEulerXYZ().ptr(), 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None); break;
			}
				
		}


	}
}

