#include "ComponentTransform.h"
#include "GameObject.h"

ComponentTransform::ComponentTransform(GameObject* parent) : 
	Component(parent), position(float3(0.0f, 0.0f, 0.0f)), rotation(Quat::identity), scale(float3(0.0f, 0.0f, 0.0f))
{
	parent->AddComponent(this);
	transform = float4x4::FromTRS(position, rotation, scale);
};

ComponentTransform::ComponentTransform(GameObject* parent, float3 position, float3 scale, Quat rotation) :
	Component(parent), scale(scale), rotation(rotation), position(position)
{
	parent->AddComponent(this);
	transform = float4x4::FromTRS(position, rotation, scale);
};

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

void ComponentTransform::UpdateMatrix(){
	transform = float4x4::FromTRS(position, rotation, scale);
}