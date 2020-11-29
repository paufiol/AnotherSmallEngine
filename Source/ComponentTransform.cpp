#include "Application.h"
#include "ModuleScene.h"
#include "ComponentTransform.h"
#include "GameObject.h"

#include "Dependencies/ImGUI/imgui.h"

ComponentTransform::ComponentTransform(GameObject* owner) : 
	Component(owner), position(float3(0.0f, 0.0f, 0.0f)), rotation(Quat::identity), scale(float3(1.0f, 1.0f, 1.0f))
{
	local_transform = float4x4::FromTRS(position, rotation, scale);
	global_transform = Quat::identity;
	type = ComponentType::Transform;
	//UpdateEulerAngles();
};

ComponentTransform::ComponentTransform(GameObject* owner, float3 position, float3 scale, Quat rotation) :
	Component(owner), scale(scale), rotation(rotation), position(position)
{
	local_transform = float4x4::FromTRS(position, rotation, scale);
	global_transform = Quat::identity;
	type = ComponentType::Transform;
	//UpdateEulerAngles();
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
	//if (updateGlobalTrans) UpdateGlobalTransform();
}

void ComponentTransform::CleanUp()
{
	
}

void ComponentTransform::UpdateTransform(float3 _position, float3 _scale, Quat _rotation)
{
	local_transform = float4x4::FromTRS(_position, _rotation, _scale);

	UpdateGlobalTransform();
}


void ComponentTransform::UpdateLocalTransform()
{
	local_transform = float4x4::FromTRS(position, rotation, scale);
	UpdateEulerAngles();
	UpdateGlobalTransform();

}

void ComponentTransform::UpdateGlobalTransform()
{
	GameObject* tempParent = owner->parent;
	
	if (tempParent != nullptr  )
	{
		global_transform = tempParent->transform->global_transform * local_transform;
	}

	UpdateTRS();

	for (uint i = 0; i < owner->children.size(); i++)
	{
		owner->children.at(i)->transform->UpdateGlobalTransform();
	}

	LOG("Global transform of %s updated", owner->name.c_str());
}

void ComponentTransform::UpdateTRS()
{
	local_transform.Decompose(position, rotation, scale);
	UpdateEulerAngles();
}

void ComponentTransform::UpdateEulerAngles()
{
	eulerRotation = rotation.ToEulerXYZ();
	eulerRotation *= RADTODEG;
}

void ComponentTransform::SetEulerRotation(float3 eulerAngles)
{
	float3 temp = eulerAngles  * DEGTORAD;
	Quat quaternion = Quat::FromEulerXYZ(temp.x, temp.y, temp.z);
	rotation =  quaternion;
	eulerRotation = temp;
	UpdateLocalTransform(); 
}


void ComponentTransform::DrawInspector() 
{
	
	if (ImGui::CollapsingHeader("Component Transform"))
	{
		if (ImGui::DragFloat3("Position", (float*)&position, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None)) { UpdateLocalTransform(); }
		if (ImGui::DragFloat3("Scale", (float*)&scale, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None)) { UpdateLocalTransform(); }
		if (ImGui::DragFloat3("Rotation", (float*)&eulerRotation, 0.06f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None)) { SetEulerRotation(eulerRotation); }
	}
}