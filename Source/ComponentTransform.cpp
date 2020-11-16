#include "ComponentTransform.h"
#include "GameObject.h"

#include "Dependencies/ImGUI/imgui.h"

ComponentTransform::ComponentTransform(GameObject* parent) : 
	Component(parent), position(float3(0.0f, 0.0f, 0.0f)), rotation(Quat::identity), scale(float3(1.0f, 1.0f, 1.0f))
{
	transform = float4x4::FromTRS(position, rotation, scale);
	type = ComponentType::Transform;
	UpdateEulerAngles();
};

ComponentTransform::ComponentTransform(GameObject* parent, float3 position, float3 scale, Quat rotation) :
	Component(parent), scale(scale), rotation(rotation), position(position)
{
	transform = float4x4::FromTRS(position, rotation, scale);
	type = ComponentType::Transform;
};

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Enable() {
	if (!this->active) {
		this->active = true;
		//ON ENABLE CODE
	}
}

void ComponentTransform::Disable() {
	if (this->active) {
		this->active = false;
		//ON DISABLE CODE
	}
}

void ComponentTransform::Update() {

}

void ComponentTransform::UpdateLocalMatrix(){
	transform = float4x4::FromTRS(position, rotation, scale);
}

void ComponentTransform::UpdateTRS()
{
	transform.Decompose(position, rotation, scale);
	UpdateEulerAngles();
}

void ComponentTransform::UpdateEulerAngles()
{
	eulerRotation = rotation.ToEulerXYZ();
	eulerRotation *= RADTODEG;
}

void ComponentTransform::SetEulerRotation(float3 eulerAngles)
{
	float3 temp = (eulerAngles - eulerRotation) * DEGTORAD;
	Quat quaternion = Quat::FromEulerXYZ(temp.x, temp.y, temp.z);
	rotation = rotation * quaternion;
	eulerRotation = eulerAngles;
	UpdateLocalMatrix();
}


void ComponentTransform::DrawInspector() {
	
	if (ImGui::CollapsingHeader("Component Transform"))
	{
		if (ImGui::DragFloat3("Position", (float*)&position, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None)) { UpdateLocalMatrix(); }
		if (ImGui::DragFloat3("Scale", (float*)&scale, 0.02f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None)) { UpdateLocalMatrix(); }
		if (ImGui::DragFloat3("Rotation", (float*)&_eulerRotation, 0.06f, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_None)) { SetEulerRotation(_eulerRotation); }
	}
}