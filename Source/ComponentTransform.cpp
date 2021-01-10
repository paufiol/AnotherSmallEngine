#include "Application.h"
#include "ModuleScene.h"
#include "ComponentTransform.h"
#include "GameObject.h"

#include "Dependencies/ImGUI/imgui.h"

ComponentTransform::ComponentTransform(GameObject* owner) : 
	Component(owner), local_position(float3(0.0f, 0.0f, 0.0f)), local_rotation(Quat::identity), local_scale(float3(1.0f, 1.0f, 1.0f))
{
	local_transform = float4x4::FromTRS(local_position, local_rotation, local_scale);
	global_transform = Quat::identity;
	global_position = float3::zero;
	global_scale = float3::one;
	global_rotation = Quat::identity;
	type = ComponentType::Transform;

};

ComponentTransform::ComponentTransform(GameObject* owner, float3 position, float3 scale, Quat rotation) :
	Component(owner), local_scale(scale), local_rotation(rotation), local_position(position)
{
	local_transform = float4x4::FromTRS(position, rotation, scale);
	global_transform = Quat::identity;
	type = ComponentType::Transform;

};

ComponentTransform::ComponentTransform(GameObject* owner, float3 position, float3 scale, Quat rotation, float3 globalPosition, float3 globalScale, Quat globalRotation) :
	Component(owner), local_scale(scale), local_rotation(rotation), local_position(position), global_scale(globalScale), global_rotation(globalRotation), global_position(globalPosition)
{
	local_transform = float4x4::FromTRS(position, rotation, scale);
	global_transform = float4x4::FromTRS(globalPosition, globalRotation, globalScale);
	type = ComponentType::Transform;

};

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Enable() 
{
	if (!this->active) {
		this->active = true;
		//ON ENABLE CODE
	}
}

void ComponentTransform::Disable() 
{
	if (this->active) {
		this->active = false;
		//ON DISABLE CODE
	}
}

void ComponentTransform::Update() 
{

}

void ComponentTransform::CleanUp()
{
	
}

void ComponentTransform::UpdateTransform(float3 _position, float3 _scale, Quat _rotation)
{
	local_transform = float4x4::FromTRS(_position, _rotation, _scale);
	UpdateEulerAngles();
	UpdateGlobalTransform();
	updatedtransform = true;
}


void ComponentTransform::UpdateLocalTransform()
{
	if (local_scale.x < 0.1f)  local_scale.x = 0.1f;
	if (local_scale.y < 0.1f)  local_scale.y = 0.1f;
	if (local_scale.z < 0.1f)  local_scale.z = 0.1f;
	
	local_transform = float4x4::FromTRS(local_position, local_rotation, local_scale);
	UpdateEulerAngles();
	UpdateGlobalTransform();
	updatedtransform = true;
}

void ComponentTransform::UpdateGlobalTransform()
{

	GameObject* tempParent = owner->parent;
	if (tempParent != nullptr)
	{
		global_transform = tempParent->transform->global_transform * local_transform;
	}
	else global_transform = local_transform;

	UpdateTRS();

	for (uint i = 0; i < owner->children.size(); i++)
	{
		owner->children.at(i)->transform->UpdateGlobalTransform();
	}
}

void ComponentTransform::UpdateTRS()
{
	global_transform.Decompose(global_position, global_rotation, global_scale);
	local_transform.Decompose(local_position, local_rotation, local_scale);
	UpdateEulerAngles();
}

void ComponentTransform::UpdateEulerAngles()
{
	local_eulerRotation = local_rotation.ToEulerXYZ();
	local_eulerRotation *= RADTODEG;
}

void ComponentTransform::SetLocalEulerRotation(float3 eulerAngles)
{
	float3 temp = eulerAngles  * DEGTORAD;
	Quat quaternion = Quat::FromEulerXYZ(temp.x, temp.y, temp.z);
	local_rotation =  quaternion;
	local_eulerRotation = temp;
	UpdateLocalTransform(); 
	UpdateEulerAngles();
}

void ComponentTransform::SetGlobalEulerRotation(float3 eulerAngles)
{
	float3 temp = eulerAngles * DEGTORAD;
	Quat quaternion = Quat::FromEulerXYZ(temp.x, temp.y, temp.z);
	global_rotation = quaternion;
	global_eulerRotation = temp;
	UpdateLocalTransform();
	UpdateEulerAngles();
}


void ComponentTransform::SetGlobalTransform(float4x4 new_matrix)
{
	global_transform = new_matrix;
	
	UpdateTRS();

	if (owner->children.empty()) return;

	for (uint i = 0; i < owner->children.size(); i++)
	{
		owner->children.at(i)->transform->UpdateGlobalTransform();
	}
}

void ComponentTransform::SetLocalTransform(float4x4 new_matrix)
{
	GameObject* tempParent = owner->parent;
	if (tempParent != nullptr)
	{
		local_transform = new_matrix;// *tempParent->transform->GetGlobalTransform().Inverted();
	}

	UpdateEulerAngles();
	UpdateGlobalTransform();
	updatedtransform = true;
}

void ComponentTransform::DrawInspector()
{
	if (ImGui::CollapsingHeader("Component Transform"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::Text("Local");
		if (ImGui::DragFloat3("Local Position", (float*)&local_position, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None)) { UpdateLocalTransform(); }
		if (ImGui::DragFloat3("Local Scale", (float*)&local_scale, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None)) { UpdateLocalTransform(); }
		if (ImGui::DragFloat3("Local Rotation", (float*)&local_eulerRotation, 0.08f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None)) { SetLocalEulerRotation(local_eulerRotation); }

		ImGui::Separator();

		ImGui::Text("Global");
		if (ImGui::DragFloat3("Global Position", (float*)&global_position, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None)) { UpdateLocalTransform(); }
		if (ImGui::DragFloat3("Global Scale", (float*)&global_scale, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None)) { UpdateLocalTransform(); }
		if (ImGui::DragFloat3("Global Rotation", (float*)&global_eulerRotation, 0.08f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None)) { SetGlobalEulerRotation(global_eulerRotation); }
	}
}