#include "ComponentTransform.h"
#include "GameObject.h"

ComponentTransform::ComponentTransform(GameObject* parent) : Component(parent) 
{
	parent->AddComponent(this);
	
	position = (0.0f, 0.0f, 0.0f);
	rotation = (0.0f, 0.0f, 0.0f);
	scale = (1.0f, 1.0f, 1.0f);
};

ComponentTransform::ComponentTransform(GameObject* parent, vec3 position, vec3 rotation, vec3 scale) :
	Component(parent), scale(scale), rotation(rotation), position(position)
{
	parent->AddComponent(this);
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