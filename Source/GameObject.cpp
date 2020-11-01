#include "GameObject.h"
#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentTexture.h"
#include "ComponentMesh.h"
#include <vector>


GameObject::GameObject(std::string name): name(name), active(true)
{
	AddComponent(new ComponentTransform(this));
}

bool GameObject::Enable() //Start up + bool toggle
{
	if (!active) {
		active = true;
		return true;
		//StartUp here...
	}
	return false;
} 

bool GameObject::Disable() //Start up + bool toggle
{
	if (active) {
		active = false;
		return true;
		//CleanUp here...
	}
	return false;
}

void GameObject::Update() //Start up + bool toggle
{
	std::vector<Component*>::iterator item = components.begin();
	bool ret = true;
	
	for (; item != components.end() && ret == true; ++item) {
		//ret = (*item)->Update();
	}
}

Component* GameObject::AddComponent(Component* component)
{
	Component* ret = nullptr;

	switch (component->type)
	{
	case(ComponentType::Error):
		//LOG("Component Type Error! Something broke...");
		break;
	case(ComponentType::Transform):
		ret = new ComponentTransform(this);
		break;
	case(ComponentType::Material):
		ret = new ComponentTexture(this);
		break;
	case(ComponentType::Mesh):
		ret = new ComponentMesh(this);
		break;

	}
	
	components.push_back(ret);
	return ret;
}

Component* GameObject::GetComponent(ComponentType type)
{
	std::vector<Component*>::iterator item = components.begin();
	
	for (; item != components.end(); ++item) {

		if ((*item)->type == type) {
			return (*item);
		}
	}

	return nullptr;
}

void GameObject::AppendChildren(GameObject* children) {
	this->children.push_back(children);
}